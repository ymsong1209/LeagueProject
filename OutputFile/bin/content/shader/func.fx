#ifndef _FUNC
#define _FUNC

#include "value.fx"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{       
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        if (g_Light2DBuffer[i].LightType == 0)
        {
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb;
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);

            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        {
            
        }
    }
}

void CalcLight2D(float3 _vWorldPos, float3 _vWorldDir, inout tLightColor _Light)
{
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        if (g_Light2DBuffer[i].LightType == 0)
        {
            float fDiffusePow = saturate(dot(-g_Light2DBuffer[i].vWorldDir.xyz, _vWorldDir));
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow;                        
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);

            // ���� �߽ɿ��� ��ü�� ���ϴ� ����
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            float fDiffusePow = saturate(dot(-vLight, _vWorldDir));
            
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fDistPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow * fDistPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        {
            
        }
    }
}


void CalcLight3D(float3 _vViewPos, float3 _vViewNormal, uint _LightIdx, inout tLightColor _LightColor)
{
    tLightInfo lightinfo = g_Light3DBuffer[_LightIdx];
    
    float3 vLightDir = (float3) 0.f;
    float fDiffusePow = 0.f;
    float fDistPow = 1.f;
    float fAnglePow = 1.f;
    
    // Directional Light
    if (0 == lightinfo.LightType)
    {
        // Light �� ViewSpace ������ ����
        vLightDir = mul(float4(lightinfo.vWorldDir.xyz, 0.f), g_matView);     
    }
    
    // PointLight
    else if (1 == lightinfo.LightType)
    {
        // Light �� ViewSpace ������ ����
        float4 vLightViewPos = mul(float4(lightinfo.vWorldPos.xyz, 1.f), g_matView);
       
        // ǥ����ġ - ���� ��ġ
        vLightDir = normalize(_vViewPos - vLightViewPos.xyz);          
        
        // �Ÿ��� ���� ���� ��ȭ
        float fDist = distance(_vViewPos, vLightViewPos.xyz);        
        fDistPow = 1.f - saturate(fDist / lightinfo.Radius);        
    }
    
    // SpotLight
    else
    {
        
    }             
    
    // Diffuse Power
    float fPow = saturate(dot(-vLightDir, _vViewNormal));
    
    // Specular ���
    float3 vViewReflect = normalize(vLightDir + 2.f * (dot(-vLightDir, _vViewNormal)) * _vViewNormal);
    
    // ī�޶󿡼� �ȼ� ������ �ٶ󺸴� �ü� ����
    float3 vEye = -normalize(_vViewPos);
    
    // �ݻ籤 ����          
    float fRelfectPow = pow(saturate(dot(vViewReflect, vEye)), 10);
    
    // ����� ����
    _LightColor.vDiffuse += lightinfo.Color.vDiffuse * fPow * fDistPow;
    _LightColor.vSpecular += lightinfo.Color.vSpecular * fRelfectPow * fDistPow;
    _LightColor.vAmbient += lightinfo.Color.vAmbient;
}




// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f,  0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f,  0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);       
    
    vUV.x += g_AccTime * 0.5f;
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    vUV.y -= (sin((_NomalizedThreadID - (g_AccTime/*�׷��� ���� �̵� �ӵ�*/)) * 2.f * 3.1415926535f * 10.f/*�ݺ��ֱ�*/) / 2.f);
    
    if( 1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if(vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if( 1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;           
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;    
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {            
            vOut += _NoiseTex[pixel + offset + int2(j, i)] * GaussianFilter[i][j];
        }
    }        
    
    _vOut = vOut;    
}

#endif
