#ifndef _BLOOMSHADER
#define _BLOOMSHADER

#include "value.fx"

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
// GrayShader
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

	float vAver = (vColor.r + vColor.g + vColor.b) / 3.f;

	vColor = float4(vAver, vAver, vAver, 1.f);

	return vColor;
}

#endif