#ifndef _MINIMAP
#define _MINIMAP

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
// MiniMapShader
// mesh : RectMesh
// DepthStencilState : NOTEST
// Domain : UI
// g_tex_0 : MiniMapTexture
// g_tex_1 : FogFilterMap
// ============================
VS_OUT VS_MiniMapShader(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_MiniMapShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
	float4 vFilterColor = float4(0.f, 0.f, 0.f, 1.f);
	
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	vFilterColor = g_tex_1.Sample(g_sam_0, _in.vUV);


	if (vFilterColor.x == 1.f)
	{
		return vOutColor;
	}


	else
	{
		float4 FinalColor = float4(vOutColor.x * 0.3, vOutColor.y * 0.3, vOutColor.z * 0.3, 1.f);

		return FinalColor;
	}

	 
	 
} 

#endif

 