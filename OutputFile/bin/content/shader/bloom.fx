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
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vAdditionColor = g_tex_1.Sample(g_sam_0, _in.vUV);
	
    if (vAdditionColor.x == 0)
    {
        float vAver = (vColor.r + vColor.g + vColor.b) / 3.f;

        vColor = float4(vAver, vAver, vAver, 1.f);

        return vColor;
	
    }
    else
    {
        vColor = float4(0.f, 255.f, 0.f, 1.f);
		
        return vColor;
    }
	
	
	
	// ===============================================================================
	
	
	float4 vOriginalColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	
    float2 vUVStep = float2(1.f / g_vec2_0.x, 1.f / g_vec2_0.y);
	
    float4 vAdditiveColor = float4(0.f, 0.f, 0.f, 0.f);
	// 5 x 5 Gaussin Filter를 적용시킬 것이다.
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            float2 UVOffset = float2((3 - i) * vUVStep.x, (3 - j) * vUVStep.y);
            vAdditiveColor += GaussianFilter[i][j] * g_tex_1.Sample(g_sam_1, (_in.vUV + UVOffset));
        }
    }
	
	// Alpha 값은 평균값으로 대체해준다.
    //vAdditiveColor.w = vAdditiveColor.w / 25;
	
    return (vOriginalColor + vAdditiveColor);
	
	
	
	/* Gray Shader 부분에 적혀 있던 코드들 (시작)
        float vAver = (vColor.r + vColor.g + vColor.b) / 3.f;

	vColor = float4(vAver, vAver, vAver, 1.f);

	return vColor;
	 Gray Shader 부분에 적혀 있던 코드들 (끝) */
	
	
}

#endif