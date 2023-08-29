#ifndef _FOG_FILTER
#define _FOG_FILTER

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWTexture2D<float> FILTER_MAP : register(u0); // Unordered Access
RWStructuredBuffer<tRayOutput> RAYINFO : register(u1); // Ray에 대한 정보들 (CenterPos 이용할 것임)

#define WIDTH           g_int_0     // 필터맵의 넓이
#define HEIGHT          g_int_1     // 필터맵의 높이
#define CntObject       g_int_2     // 시야를 가진 오브젝트 개수
#define CntRayPerObject g_int_3     // 오브젝트가 쏠 레이의 개수


// CenterPos.xz는 월드 pos 로 되어 있다. 월드는 좌하단(0,0)부터 우상단으로(롤맵 가로2200, 롤맵 세로2200 가정) ↗
// _iThreadId.x, _iThreradID.y는 필터맵 텍스처 내 좌표다. 좌상단(0,0)부터 우하단으로(2048,2048)↘
// => 좌표축 일치x -> CenterPos를 _iThreadId.x, _iThreadId.y 좌표계로 변환하는 함수가 필요하다. 
float2 ConvertToThreadCoords(float2 centerPos, int threadWidth, int threadHeight, int worldMapWidth, int worldMapHeight)
{
    float threadX = (centerPos.x / worldMapWidth) * threadWidth;
    float threadY = (1.0 - (centerPos.y / worldMapHeight)) * threadHeight;
    return float2(threadX, threadY);
}

[numthreads(32, 32, 1)]
void CS_FogFilterShader(int3 _iThreadID : SV_DispatchThreadID)
{
    // init
    FILTER_MAP[_iThreadID.xy] = float4(0.f, 0.f, 0.f, 0.f);
     
    // 우리가 만들 안개 필터맵 텍스처 크기를 벗어나면 버린다.
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y)
        return;
    
    // 2차원 인덱스 좌표를 1차원 인덱스로 계산
    //uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;
    
    float isVisible = 0.f; 
    
    // 시야 판별
    for (int i = 0; i < CntObject; ++i) // 오브젝트 개수만큼 
    {
        if (isVisible)
            break;
        
        // RAYINFO 버퍼에서 해당 인덱스 오브젝트 정보를 읽어온다.
        tRayOutput rayInfo = RAYINFO[i * CntRayPerObject];
        
        // 현재 텍스처에서의 Pos 
        float2 curThreadPos = float2(_iThreadID.x, _iThreadID.y);
        
        // 롤 World Center Pos를 텍스처 내 UV로 변경. 
        float2 rayCenterPos = ConvertToThreadCoords(rayInfo.CenterPos.xz, WIDTH, HEIGHT, 2200, 2200);
        
        // rayInfo의 ray 시야 범위도 World 길이 기준이다. => 텍스처 내 길이로 변환
        float radiusObjectVision = (rayInfo.MaxRadius / 2200.f) * WIDTH; // 혹은 HEIGHT 사용

        
        // 1. 현재 픽셀이(curThreadPos) 오브젝트의 시야 범위내에 있는지 판별한다. (컬링용)
        // 오브젝트 중점과 현재 픽셀 사이의 길이가 오브젝트의 시야 범위(시야 길이 반지름)보다 작으면 -> 원 내부O
        if (length(curThreadPos - rayCenterPos) <= radiusObjectVision)
        {            
            // 이제 원 안에 있음이 확실하다. 
            // 원을 n등분으로 쪼개서 피자 조각이라 가정한다. 
            // 현제 픽셀이 어떤 피자조각 내부인지 판별. 
            // => 현재 픽셀이 몇도에 위치하는지, n번째 레이가 몇도인지, n+1번째 레이가 몇도인지 알아야한다. 
                
            // 피자 한 조각의 각도(pizzaTheta)
            float pizzaTheta = 2.f * 3.141592f / (float) CntRayPerObject; 
               
                
            float2 vectorA = normalize(curThreadPos - rayCenterPos); // center에서 픽셀을 향하는 벡터
            float2 vectorB = float2(1.0f, 0.0f); // 0도 
                
            float cosCurTheta = dot(vectorA, vectorB);
            float curTheta = acos(cosCurTheta); // CenterPos를 기준으로 픽셀이 위치한 각도가 0~180인지 180~360인지 판단 불가능.
                
            //픽셀 위치를 통해 Acos을 한 각도에 PI를 더할지 판별
            if (rayCenterPos.y < curThreadPos.y)
            {
                curTheta = 2 * 3.141592f - curTheta;
            }
           
                
            // 이제 우리는 피자조각 세타도 알고(PizzaTheta), 픽셀의 세타도 안다.(curTheta)
            int   index     = int(curTheta / pizzaTheta);
            float nTheta    = index * pizzaTheta;
            float nextTheta = (index + 1) * pizzaTheta;
                
            // 2. n 각도  <= 현재 픽셀  < n+1 각도 일시, 현제 픽셀은 n번째 피자조각 내부이다. (nTheta <= curTheta < nextTheta)
            tRayOutput nThRayInfo = RAYINFO[i * CntRayPerObject + index];
            if (nTheta <= curTheta && curTheta < nextTheta)
            {
                float RadiusConvertedToTexture = (float) nThRayInfo.Radius / 2200.f * (float) WIDTH; // 해당 레이의 반지름도 텍스처 내 길이로 변경
                    
                // 3. 중점에서 현재 픽셀까지의 거리가, n번째 피자가 가지는 시야범위 반지름 길이보다 내부에 있는지 판단
                if (length(curThreadPos - rayCenterPos) <= RadiusConvertedToTexture)
                {
                    isVisible = 1.f; // 레이가 시야 안에 있음 : 1 
                    break; // 한번이라도 시야 안에 있으면 더 이상 확인할 필요 없음
                }
            }
        }
    }
    
    // 필터맵 구조화 버퍼에 레이의 시야 여부를 기록
    if (isVisible)
        FILTER_MAP[_iThreadID.xy] = 1.f;
    
    else
        FILTER_MAP[_iThreadID.xy] = 0.f;
   
    
}


#endif