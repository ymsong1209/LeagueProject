#ifndef _FOG_FILTER
#define _FOG_FILTER

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWTexture2D<float> FILTER_MAP : register(u0); // Unordered Access
StructuredBuffer<tRayOutput> RAYINFO : register(t16); // Ray에 대한 정보들 (CenterPos 이용할 것임)

#define WIDTH           g_int_0
#define HEIGHT          g_int_1
#define CntObject       g_int_2
#define CntRayPerObject g_int_3

#define LOLMAPWIDTH     3000
#define LOLMAPHEIGHT    3000

float PI = 3.141592;
// CenterPos.xz는 월드 pos 로 되어 있다. 월드는 좌하단(0,0)부터 우상단으로(롤맵가로3000, 롤맵세로3000 가정) ↗
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
    
    
    // 텍스처 크기 벗어나면 버림
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y)
    {
        return;
    }
    
    // 2차원 인덱스 좌표를 1차원 인덱스로 계산
    //uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;
    
    float isVisible = 0; 
    
    // 시야 판별
    for (int i = 0; i < CntObject; ++i) // 오브젝트 개수만큼 
    {
        // RAYINFO 버퍼에서 해당 인덱스 오브젝트 정보 읽어온다.
        tRayOutput rayInfo = RAYINFO[i * CntRayPerObject];
        
        float2 curThreadPos = float2(_iThreadID.x, _iThreadID.y);
        
        // World Center Pos를 텍스처 내 UV로 변경. 
        float2 rayCenterPos = ConvertToThreadCoords(rayInfo.CenterPos.xz, WIDTH, HEIGHT, 3000, 3000);
        
        // ray 시야 범위도 World 길이 기준이니까 텍스처 내 길이로 변환
        // 지금 RayRange를 500으로 가정.
        float radiusObjectVision = (500 / 3000.0) * WIDTH; // 혹은 HEIGHT 사용

        // 1. 이 픽셀이 오브젝트의 시야 범위(시야 길이 반지름)보다 작으면 -> 원 안에 있음. (컬링용)
        if (length(curThreadPos - rayCenterPos) <= radiusObjectVision)
        {
            for (int j = 0; j < CntRayPerObject-1; ++j)  // 오브젝트가 가진 레이 개수 -1만큼 돈다.
            {
                // 이제 원 안에 있으니까, 360등분으로 쪼개서 360분의 1조각에서 반지름 길이보다 내부에 있는지 판단한다. 
                tRayOutput nThRayInfo = RAYINFO[i * CntRayPerObject + j];
                tRayOutput nextThRayInfo = RAYINFO[i * CntRayPerObject + j + 1];
                
                // 2. 이제 원안에서 어디 피자조각 내부인지알아야 한다.
                // 어디 피자조각 내부인지 알기 위해서는 
                // 현재 픽셀이 몇도에 위치하는지, n번째 레이가 몇도인지, n+1번째 레이가 몇도인지 알아야한다.
                // 현재 픽셀이 n+1 각도보다 작고, n 각도보다 크면 n번째 피자조각
                
                float pizzaTheta = 2*PI / CntRayPerObject; // 피자 한 조각의 각도
                
                float2 vectorA = normalize(curThreadPos - rayCenterPos); // center에서 픽셀을 향하는 벡터
                float2 vectorB = float2(1, 0); // 0도 
                
                float cosCurTheta = dot(vectorA, vectorB); 
                float curTheta = acos(cosCurTheta); // CenterPos를 기준으로 픽셀이 위치한 각도가 0~180인지 180~360인지 판단 불가능.
                
                // 외적한 결과의 길이가 sin(theta)
                float3 sinCurTheta = cross(float3(vectorA, 0), float3(vectorB, 0)); 
                sinCurTheta = length(sinCurTheta);
                
                float sign = asin(sinCurTheta); // asin결과가 양수면 0~180도 사이고, 음수면 180~360도 사이다. 
                
                if(sign < 0) // 음수면 360도에서 세타를 빼준다.
                    curTheta = 2 * PI - curTheta;
                
                // 이제 우리는 피자조각 세타도 알고(PizzaTheta), 픽셀의 세타도 안다.(curTheta)
                float nTheta = j * pizzaTheta;
                float nextTheta = (j + 1) * pizzaTheta;
                
                // 3. 현재 각도가 어떤 ?번째 피자조각 내부일때, nTheta <= curTheta < nextTheta
                if (nTheta <= curTheta && curTheta < nextTheta) 
                {
                    float rayRadiusUV = nThRayInfo.Radius / 3000 * WIDTH; // 해당 레이의 반지름도 텍스처 내 길이로 변경
                    
                    // ?번째 피자가 가지는 시야범위 반지름 길이보다 내부에 있는지 판단
                    if (length(curThreadPos - rayCenterPos) <= rayRadiusUV)
                    {
                        isVisible = 1; // 레이가 시야 안에 있음 : 1 (나중에는 알파를 0으로 하도록 할까 고민중)
                        break; // 한번이라도 시야 안에 있으면 더 이상 확인할 필요 없음
                    }
                }
         
                
            }
        }
    }
    
    // 필터맵 구조화 버퍼에 레이의 시야 여부를 기록
    if(isVisible)
        FILTER_MAP[_iThreadID.xy] = float4(255.0, 0.0, 0.0, 255.0);
    else
        FILTER_MAP[_iThreadID.xy] = float4(255.0, 255.0, 0.0, 255.0);
}


#endif