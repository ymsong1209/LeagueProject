#ifndef _GLOWSHADER
#define _GLOWSHADER

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ============================
// GlowShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// Animation ฐüทร
#define bAnimUse                    g_int_0    // Anim Use
#define LeftTop                     g_vec2_0   // AnimAtlas LeftTop
#define Slice                       g_vec2_1   // AnimAtlas Slice
#define Offset                      g_vec2_2
#define BackSize                    g_vec2_3


VS_OUT VS_GLOWSHADER(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;

    return output;
}


float4 PS_GLOWSHADER(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 0.f, 1.f);


    return vOutColor;
}

#endif