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

// g_tex_2 : PlayerIcon1
// g_tex_3 : PlayerIcon2
// g_tex_4 : PlayerIcon3
// g_tex_5 : PlayerIcon4

// g_vec2_0 : PlayerIcon1
// g_vec2_1 : PlayerIcon2
// g_vec2_2 : PlayerIcon3
// g_vec2_3 : PlayerIcon4


#define PlayerPos1                  g_vec2_0  
#define PlayerPos2                  g_vec2_1  
#define PlayerPos3                  g_vec2_2  
#define PlayerPos4                  g_vec2_3  

#define Playericon1                  g_tex_2  
#define Playericon2                  g_tex_3  
#define Playericon3                  g_tex_4  
#define Playericon4                  g_tex_5  

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
    
    float mapSize = 512.f;
    float2 PlayerPosUV1 = float2(PlayerPos1.x / mapSize, 1 - (PlayerPos1.y / mapSize));
    float2 PlayerPosUV2 = float2(PlayerPos2.x / mapSize, 1 - (PlayerPos2.y / mapSize));
    float2 PlayerPosUV3 = float2(PlayerPos3.x / mapSize, 1 - (PlayerPos3.y / mapSize));
    float2 PlayerPosUV4 = float2(PlayerPos4.x / mapSize, 1 - (PlayerPos4.y / mapSize));
	
    vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
	vFilterColor = g_tex_1.Sample(g_sam_0, _in.vUV);

	if (vFilterColor.x != 1.f)
    {
        vOutColor = float4(vOutColor.x * 0.3, vOutColor.y * 0.3, vOutColor.z * 0.3, 1.f);
        return vOutColor;
    }
  
    float2 iconSize = float2(0.1, 0.1);
    
	// 작은 텍스처 아이콘 출력 위치 조정
    float2 uvIcon = (_in.vUV - PlayerPosUV1) / iconSize; // 아이콘의 UV 좌표를 조정하여 크기 비율 적용
    float2 uvIcon1 = (_in.vUV - PlayerPosUV2) / iconSize; 
    float2 uvIcon2 = (_in.vUV - PlayerPosUV3) / iconSize; 
    float2 uvIcon3 = (_in.vUV - PlayerPosUV4) / iconSize; 
    
    // 해당 위치에만 작은 텍스처 아이콘 샘플링
    float4 texIconColor1 = 0;
    float4 texIconColor2 = 0;
    float4 texIconColor3 = 0;
    float4 texIconColor4 = 0;
    
    if (uvIcon.x >= 0 && uvIcon.x <= 1 && uvIcon.y >= 0 && uvIcon.y <= 1)
        texIconColor1 = Playericon1.Sample(g_sam_0, uvIcon);
    
    if (uvIcon1.x >= 0 && uvIcon1.x <= 1 && uvIcon1.y >= 0 && uvIcon1.y <= 1)
        texIconColor2 = Playericon2.Sample(g_sam_0, uvIcon1);
    
    if (uvIcon2.x >= 0 && uvIcon2.x <= 1 && uvIcon2.y >= 0 && uvIcon2.y <= 1)
        texIconColor3 = Playericon3.Sample(g_sam_0, uvIcon2);
    
    if (uvIcon3.x >= 0 && uvIcon3.x <= 1 && uvIcon3.y >= 0 && uvIcon3.y <= 1)
        texIconColor4 = Playericon4.Sample(g_sam_0, uvIcon3);

    // 텍스처 알파 블렌딩을 사용하여 맵 이미지에 작은 아이콘을 덧그리기
    vOutColor = lerp(vOutColor, texIconColor1, texIconColor1.a);
    vOutColor = lerp(vOutColor, texIconColor2, texIconColor2.a);
    vOutColor = lerp(vOutColor, texIconColor3, texIconColor3.a);
    vOutColor = lerp(vOutColor, texIconColor4, texIconColor4.a);
    
    return vOutColor;
} 

#endif

 