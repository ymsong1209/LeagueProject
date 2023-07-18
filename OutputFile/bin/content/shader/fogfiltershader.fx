#ifndef _FOG_FILTER
#define _FOG_FILTER

#include "value.fx"
#include "struct.fx"
#include "func.fx"


RWTexture2D<float> FILTER_MAP : register(u0); // Unordered Access
RWStructuredBuffer<tRayOutput> RAYINFO : register(u1); // Ray�� ���� ������ (CenterPos �̿��� ����)

#define WIDTH           g_int_0
#define HEIGHT          g_int_1
#define CntObject       g_int_2
#define CntRayPerObject g_int_3



// CenterPos.xz�� ���� pos �� �Ǿ� �ִ�. ����� ���ϴ�(0,0)���� ��������(�Ѹʰ���3000, �Ѹʼ���3000 ����) ��
// _iThreadId.x, _iThreradID.y�� ���͸� �ؽ�ó �� ��ǥ��. �»��(0,0)���� ���ϴ�����(2048,2048)��
// => ��ǥ�� ��ġx -> CenterPos�� _iThreadId.x, _iThreadId.y ��ǥ��� ��ȯ�ϴ� �Լ��� �ʿ��ϴ�. 
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
    
    
    // �ؽ�ó ũ�� ����� ����
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y)
    {
        return;
    }
    
    // 2���� �ε��� ��ǥ�� 1���� �ε����� ���
    //uint iIdx = (_iThreadID.y * WIDTH) + _iThreadID.x;
    
    float isVisible = 0.f; 
    
    // �þ� �Ǻ�
    for (int i = 0; i < CntObject; ++i) // ������Ʈ ������ŭ 
    {
        // RAYINFO ���ۿ��� �ش� �ε��� ������Ʈ ���� �о�´�.
        tRayOutput rayInfo = RAYINFO[i * CntRayPerObject];
        
        float2 curThreadPos = float2(_iThreadID.x, _iThreadID.y);
        
        // World Center Pos�� �ؽ�ó �� UV�� ����. 
        float2 rayCenterPos = ConvertToThreadCoords(rayInfo.CenterPos.xz, WIDTH, HEIGHT, 3000, 3000);
        
        // ray �þ� ������ World ���� �����̴ϱ� �ؽ�ó �� ���̷� ��ȯ
        // ���� RayRange�� 500���� ����.
        float radiusObjectVision = (500 / 3000.0) * WIDTH; // Ȥ�� HEIGHT ���

        // 1. �� �ȼ��� ������Ʈ�� �þ� ����(�þ� ���� ������)���� ������ -> �� �ȿ� ����. (�ø���)
        if (length(curThreadPos - rayCenterPos) <= radiusObjectVision)
        {            
            for (int j = 0; j < CntRayPerObject; ++j)  // ������Ʈ�� ���� ���� ���� -1��ŭ ����.
            {
                // ���� �� �ȿ� �����ϱ�, 360������� �ɰ��� 360���� 1�������� ������ ���̺��� ���ο� �ִ��� �Ǵ��Ѵ�. 
                tRayOutput nThRayInfo = RAYINFO[i * CntRayPerObject + j];
                
                // 2. ���� ���ȿ��� ��� �������� ���������˾ƾ� �Ѵ�.
                // ��� �������� �������� �˱� ���ؼ��� 
                // ���� �ȼ��� ��� ��ġ�ϴ���, n��° ���̰� �����, n+1��° ���̰� ����� �˾ƾ��Ѵ�.
                // ���� �ȼ��� n+1 �������� �۰�, n �������� ũ�� n��° ��������
                
                float pizzaTheta = 2.f * 3.141592f / (float) CntRayPerObject; // ���� �� ������ ����
                
                
                RAYINFO[i * CntRayPerObject + j].pad[0] = degrees(pizzaTheta);
               
                
                
                float2 vectorA = normalize(curThreadPos - rayCenterPos); // center���� �ȼ��� ���ϴ� ����
                float2 vectorB = float2(1.0f, 0.0f); // 0�� 
                
                float cosCurTheta = dot(vectorA, vectorB); 
                float curTheta = acos(cosCurTheta); // CenterPos�� �������� �ȼ��� ��ġ�� ������ 0~180���� 180~360���� �Ǵ� �Ұ���.
                
                //�ȼ� ��ġ�� ���� Acos�� �� ������ PI�� ������ �Ǻ�
                if (rayCenterPos.y < curThreadPos.y)
                {
                    curTheta = 2 * 3.141592f - curTheta;
                }
                
                
                
                //// ������ ����� ���̰� sin(theta)
                //float3 CrossResult = cross(float3(vectorA, 0), float3(vectorB, 0)); 
                //float3 CrossResultLength = length(CrossResult);
                
                //float sign = asin(CrossResultLength); // asin����� ����� 0~180�� ���̰�, ������ 180~360�� ���̴�. 
                
                //if(sign < 0) // ������ 360������ ��Ÿ�� ���ش�.
                //    curTheta = 2 * PI - curTheta;
                
                // ���� �츮�� �������� ��Ÿ�� �˰�(PizzaTheta), �ȼ��� ��Ÿ�� �ȴ�.(curTheta)
                float nTheta = j * pizzaTheta;
                float nextTheta = (j + 1) * pizzaTheta;
                
                // 3. ���� ������ � ?��° �������� �����϶�, nTheta <= curTheta < nextTheta
                if (nTheta <= curTheta && curTheta < nextTheta) 
                {
                    float RadiusConvertedToTexture = (float) nThRayInfo.Radius / 3000.f * (float) WIDTH; // �ش� ������ �������� �ؽ�ó �� ���̷� ����
                    
                    // ?��° ���ڰ� ������ �þ߹��� ������ ���̺��� ���ο� �ִ��� �Ǵ�
                    if (length(curThreadPos - rayCenterPos) <= RadiusConvertedToTexture)
                    {
                        isVisible = 1.f; // ���̰� �þ� �ȿ� ���� : 1 (���߿��� ���ĸ� 0���� �ϵ��� �ұ� �����)
                        break; // �ѹ��̶� �þ� �ȿ� ������ �� �̻� Ȯ���� �ʿ� ����
                    }
                }
         
                
            }
        }
    }
    
    // ���͸� ����ȭ ���ۿ� ������ �þ� ���θ� ���
    if (isVisible)
        FILTER_MAP[_iThreadID.xy] = 1.f;
    //float4(255.0, 0.0, 0.0, 255.0);
    else
        FILTER_MAP[_iThreadID.xy] = 0.f;
    //float4(255.0, 255.0, 0.0, 255.0);
    
     
    
    //if ((_iThreadID.x > 500 && _iThreadID.x < 1000) && (_iThreadID.y > 500 && _iThreadID.y < 1000))
    //{
    //    FILTER_MAP[_iThreadID.xy] = float4(0.0, 0.0, 255.0, 255.0);
    //}
    //FILTER_MAP[_iThreadID.xy] = float4(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
    //float4(1.0, 1.0, 0.0, 1.0);
    //float4(100.0, 255.0, 0.0, 255.0);
}


#endif