#ifndef _BLOOMSHADER
#define _BLOOMSHADER

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
// Bloom Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// g_tex_1 : Emissive Texture
// g_vec2_0 : RenderTargetTex Resolution
// ============================
VS_OUT VS_BloomShader(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_BloomShader(VS_OUT _in) : SV_Target
{
	float4 vOriginalColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	
    float2 vUVStep = float2(5.f / g_vec2_0.x, 5.f / g_vec2_0.y);
	
    float4 vAdditiveColor = float4(0.f, 0.f, 0.f, 0.f);
	// 5 x 5 Gaussin Filter를 적용시킬 것이다.
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            float2 UVOffset = float2((i - 4) * vUVStep.x, (j - 4) * vUVStep.y);
            vAdditiveColor += GaussianFilter3[i][j] * g_tex_1.Sample(g_sam_0, (_in.vUV + UVOffset));
        }
    }	
    return (vOriginalColor + vAdditiveColor);
}

#endif