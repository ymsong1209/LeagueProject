#ifndef _FOG_FILTER
#define _FOG_FILTER

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWStructuredBuffer<int> FILTER_MAP : register(u0); // Unordered Access
StructuredBuffer<tRayOutput> RAYINFO : register(t16); // Ray에 대한 정보들 (CenterPos 이용할 것임)

#define WIDTH           g_int_0
#define HEIGHT          g_int_1
#define CntObject       g_int_2
#define CntRayPerObject g_int_3

[numthreads(32, 32, 1)]
void CS_FogFilterShader(int3 _iThreadID : SV_DispatchThreadID)
{
    // 텍스처 크기 벗어나면 버림
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y)
    {
        return;
    }
    
    // 2차원 인덱스 좌표를 1차원 인덱스로 계산
    uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;
    
    // RAYINFO 버퍼에서 해당 인덱스에서 오브젝트 정보 읽어온다.
    //tRayOutput rayInfo = RAYINFO[iIdx];
    
    int isVisible = 0; 
    
    // 시야 판별
    for (int i = 0; i < CntObject; ++i)
    {
        for (int j = 0; j < CntRayPerObject; ++j)
        {
            tRayOutput rayInfo = RAYINFO[i*j];
            
            // 레이의 IntersectPos가 CenterPos에서 일정 거리(Radius) 이내에 있는지 확인
            if (length(rayInfo.IntersectPos - rayInfo.CenterPos) <= rayInfo.Radius)
            {
                isVisible = 1; // 레이가 시야 안에 있음 : 1 (나중에는 알파를 0으로 하도록 할까 고민중)
                break; // 한번이라도 시야 안에 있으면 더 이상 확인할 필요 없음
            }
            
        }

    }
    
    // 필터맵 구조화 버퍼에 레이의 시야 여부를 기록
    FILTER_MAP[iIdx] = isVisible;
}


#endif