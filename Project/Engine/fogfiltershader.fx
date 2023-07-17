#ifndef _FOG_FILTER
#define _FOG_FILTER

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWStructuredBuffer<int> FILTER_MAP : register(u0); // Unordered Access
StructuredBuffer<tRayOutput> RAYINFO : register(t16); // Ray�� ���� ������ (CenterPos �̿��� ����)

#define WIDTH           g_int_0
#define HEIGHT          g_int_1
#define CntObject       g_int_2
#define CntRayPerObject g_int_3

[numthreads(32, 32, 1)]
void CS_FogFilterShader(int3 _iThreadID : SV_DispatchThreadID)
{
    // �ؽ�ó ũ�� ����� ����
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y)
    {
        return;
    }
    
    // 2���� �ε��� ��ǥ�� 1���� �ε����� ���
    uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;
    
    // RAYINFO ���ۿ��� �ش� �ε������� ������Ʈ ���� �о�´�.
    //tRayOutput rayInfo = RAYINFO[iIdx];
    
    int isVisible = 0; 
    
    // �þ� �Ǻ�
    for (int i = 0; i < CntObject; ++i)
    {
        for (int j = 0; j < CntRayPerObject; ++j)
        {
            tRayOutput rayInfo = RAYINFO[i*j];
            
            // ������ IntersectPos�� CenterPos���� ���� �Ÿ�(Radius) �̳��� �ִ��� Ȯ��
            if (length(rayInfo.IntersectPos - rayInfo.CenterPos) <= rayInfo.Radius)
            {
                isVisible = 1; // ���̰� �þ� �ȿ� ���� : 1 (���߿��� ���ĸ� 0���� �ϵ��� �ұ� �����)
                break; // �ѹ��̶� �þ� �ȿ� ������ �� �̻� Ȯ���� �ʿ� ����
            }
            
        }

    }
    
    // ���͸� ����ȭ ���ۿ� ������ �þ� ���θ� ���
    FILTER_MAP[iIdx] = isVisible;
}


#endif