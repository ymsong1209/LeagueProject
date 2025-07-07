#ifndef _FUNC
#define _FUNC

#include "value.fx"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        // Directional light
        if (g_Light2DBuffer[i].LightType == 0)
        {
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb;
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        // Point Light
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);

            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow;
        }
        // Spot Light
        else if (g_Light2DBuffer[i].LightType == 2)
        {
             //광원의 x축으로 타원형 빛을 발사한다.
            float3 vLightDir = float3(g_Light2DBuffer[i].vWorldDir.xy, 0.f);
            //광원의 중심의 WorldPosition
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            //광원의 빛을 받는 물체의 Position
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
           // 광원 중심에서 물체를 향하는 방향
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            
            
            if (acos(dot(vLight, vLightDir)) < g_Light2DBuffer[i].Angle / 2.f)
            {
                float fDistance = abs(distance(vWorldPos, vLightWorldPos));
                //광원에서 멀리 있는 물체는 흐려지게 함
                float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
                //양옆으로 갈수록 흐려지게 함
                float fAnglePow = saturate(1.f - acos(dot(vLight, vLightDir)) / (g_Light2DBuffer[i].Angle / 2.f));
        
                _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow * fAnglePow;
            }
            else
            {
                _Light.vDiffuse.rgb = float3(0.f, 0.f, 0.f);
            }
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

            // 광원 중심에서 물체를 향하는 방향
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            float fDiffusePow = saturate(dot(-vLight, _vWorldDir));
            
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fDistPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow * fDistPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        {
            
            //광원의 x축으로 타원형 빛을 발사한다.
            float3 vLightDir = float3(g_Light2DBuffer[i].vWorldDir.xy, 0.f);
            //광원의 중심의 WorldPosition
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            //광원의 빛을 받는 물체의 Position
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
           // 광원 중심에서 물체를 향하는 방향
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            float fDiffusePow = saturate(dot(-vLight, _vWorldDir));
            
            if (acos(dot(vLight, vLightDir)) < g_Light2DBuffer[i].Angle / 2.f)
            {
                float fDistance = abs(distance(vWorldPos, vLightWorldPos));
                //광원에서 멀리 있는 물체는 흐려지게 함
                float fDistPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
                //양옆으로 갈수록 흐려지게 함
                float fAnglePow = saturate(1.f - acos(dot(vLight, vLightDir)) / (g_Light2DBuffer[i].Angle / 2.f));
        
                _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow * fDistPow * fAnglePow;
            }
            else
            {
                _Light.vDiffuse.rgb = float3(0.f, 0.f, 0.f);
            }
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
        // Light 의 ViewSpace 에서의 방향
        vLightDir = mul(float4(lightinfo.vWorldDir.xyz, 0.f), g_matView);
    }
    
    // PointLight
    else if (1 == lightinfo.LightType)
    {
        // Light 의 ViewSpace 에서의 방향
        float4 vLightViewPos = mul(float4(lightinfo.vWorldPos.xyz, 1.f), g_matView);
       
        // 표면위치 - 광원 위치
        vLightDir = normalize(_vViewPos - vLightViewPos.xyz);
        
        // 거리에 따른 세기 변화
        float fDist = distance(_vViewPos, vLightViewPos.xyz);
        fDistPow = 1.f - saturate(fDist / lightinfo.Radius);
    }
    
    // SpotLight
    else
    {
        // Light 의 ViewSpace 에서의 방향
        float4 vLightViewPos = mul(float4(lightinfo.vWorldPos.xyz, 1.f), g_matView);
        
        float4 vLightViewDir = normalize(mul(float4(lightinfo.vWorldDir.xyz, 0.f), g_matView));
        
        // 광원->물체표면
        vLightDir = normalize(_vViewPos - vLightViewPos.xyz);
        
        
        if (acos(saturate(dot(vLightDir, vLightViewDir.xyz))) <= lightinfo.InnerAngle / 2.f)
        {
                // 거리에 따른 세기 변화
            float fDist = distance(_vViewPos, vLightViewPos.xyz);
            fDistPow = 1.f - saturate(fDist / lightinfo.Radius);
                
               
        }
        else if (acos(saturate(dot(vLightDir, vLightViewDir.xyz))) <= lightinfo.Angle / 2.f)
        {
                // 거리에 따른 세기 변화
            float fDist = distance(_vViewPos, vLightViewPos.xyz);
            fDistPow = 1.f - saturate(fDist / lightinfo.Radius);
                
            fDistPow *= (1.f - ((acos(saturate(dot(vLightDir, vLightViewDir.xyz)))) - lightinfo.InnerAngle / 2.f) /
                            (lightinfo.Angle / 2.f - lightinfo.InnerAngle / 2.f));
        }
        else
        {
            fDistPow = 0.f;
        }
    }
    
    // Diffuse Power
    float fPow = saturate(dot(-vLightDir, _vViewNormal));
    
    // Specular 계산
    float3 vViewReflect = normalize(vLightDir + 2.f * (dot(-vLightDir, _vViewNormal)) * _vViewNormal);
    
    // 카메라에서 픽셀 지점을 바라보는 시선 벡터
    float3 vEye = -normalize(_vViewPos);
    
    // 반사광 세기          
    float fReflectPow = pow(saturate(dot(vViewReflect, vEye)), 10);
    
    // 결과값 저장
    _LightColor.vDiffuse += lightinfo.Color.vDiffuse * fPow * fDistPow;
    _LightColor.vSpecular += lightinfo.Color.vSpecular * fReflectPow * fDistPow;
    _LightColor.vAmbient += lightinfo.Color.vAmbient;
}




// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_AccTime * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_AccTime /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
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



int IntersectsLay(float3 _vertices[3], float3 _vStart, float3 _vDir, out float3 _vCrossPoint, out float _fResult)
{
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;

    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _vDir);
        
    float3 w0 = _vStart - _vertices[0].xyz;
    float a = -dot(normal, w0);
    float t = a / b;

    _fResult = t;

    float3 p = _vStart + t * _vDir;

    _vCrossPoint = p;

    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }

    return 1;
}

float GetTessFactor(float3 _vPos, int _iMinLevel, int _iMaxLevel, float _MinDistance, float _MaxDistance)
{
    float fDistance = abs(length(_vPos));

    if (_MaxDistance < fDistance)
    {
        return 0.f;
    }
    else if (fDistance < _MinDistance)
    {
        return _iMaxLevel;
    }
    else
    {
        float fLevel = _iMaxLevel - (_iMaxLevel - _iMinLevel) * ((fDistance - _MinDistance) / (_MaxDistance - _MinDistance));

        return fLevel;
    }
}


matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}



#endif
