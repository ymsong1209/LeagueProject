#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// =======================
// SkyBox Shader
// mesh : Sphere
// g_tex_0 : Output Texture

// g_int_0 : SKY Box Type ( 0 : Sphere, 1 : Cube )
// =======================

struct VS_IN
{
    float3 vPos : POSITION;  
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;    
    float2 vUV : TEXCOORD;
    float3 vLocalPos : POSITION;
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    // ViewSpace 로 취급    
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);
    output.vPosition = mul(float4(vViewPos.xyz, 1.f), g_matProj);    
    output.vPosition.z = output.vPosition.w;   
    output.vLocalPos = _in.vPos;
    
    output.vUV = _in.vUV;
    
    return output;
}

// Rasterizer

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{    
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);    
    
    // SkyBox 가 Sphere 타입인 경우
    if (g_int_0 == 0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    // SkyBox 가 Cube 타입인 경우
    else if (g_int_0 == 1)
    {
        vOutColor = g_cube_0.Sample(g_sam_0, _in.vLocalPos);
    }
    
    return vOutColor;
}



#endif
