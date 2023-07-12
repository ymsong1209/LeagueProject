#ifndef _WORLDMAP
#define _WORLDMAP

#include "value.fx"
#include "func.fx"
#include "struct.fx"

// ========================
// Worldmap Shader
//
// g_tex_0 : Color Texture
// g_tex_1 : Normal Texture
// g_float_0 : LayerNum
// ========================


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
   
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

VS_OUT VS_worldmap_Deferred(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vPosition = float4(_in.vPos, 1.f);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));
    
    return output;
}

// GeometryShader 사용
struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

// 판단 함수: 점이 카메라 절두체 내부에 있는지 확인
bool IsPointInsideFrustum(float3 vPoint)
{
    // 카메라 절두체 판단 로직을 구현하여 점이 절두체 내부에 있는지 확인하는 로직 작성
    // ...
    for (int i = 0; i < 6; ++i)
    {
        float3 vNormal = g_CamFrustumBuffer[i].xyz;
        vNormal = normalize(vNormal);
        
		// 임의의 점과 해당 평면의 법선벡터와 내적
        float fDot = dot(vNormal, vPoint);
        
		//내적한 값과 원점에서 해당 평면까지의 최단거리(D) 와 비교
        if (fDot + g_CamFrustumBuffer[i].w > 0) 
            return false; // 프러스텀 외부-> 정점 버림
    }
    // 예시: 절두체 내부에 있다고 가정
    return true;
}
[maxvertexcount(3)]
void GS_worldmap_Deferred(triangle VS_OUT _in[3], inout TriangleStream<GS_OUT> _outstream)
{
    float3 vPos0 = mul(_in[0].vPosition, g_matWorld).xyz;
    float3 vPos1 = mul(_in[1].vPosition, g_matWorld).xyz;
    float3 vPos2 = mul(_in[2].vPosition, g_matWorld).xyz;

    // 패치 내의 정점들을 내부 판단하여 3개의 정점으로 변환
    if (IsPointInsideFrustum(vPos0) ||
        IsPointInsideFrustum(vPos1) ||
        IsPointInsideFrustum(vPos2))
    {
        GS_OUT vertex;

        vertex.vPosition = mul(_in[0].vPosition, g_matWVP);
        vertex.vUV = _in[0].vUV;
        vertex.vViewPos = _in[0].vViewPos;
        vertex.vViewTangent = _in[0].vViewTangent;
        vertex.vViewNormal = _in[0].vViewNormal;
        vertex.vViewBinormal = _in[0].vViewBinormal;
        _outstream.Append(vertex);

        vertex.vPosition = mul(_in[1].vPosition, g_matWVP);
        vertex.vUV = _in[1].vUV;
        vertex.vViewPos = _in[1].vViewPos;
        vertex.vViewTangent = _in[1].vViewTangent;
        vertex.vViewNormal = _in[1].vViewNormal;
        vertex.vViewBinormal = _in[1].vViewBinormal;
        _outstream.Append(vertex);

        vertex.vPosition = mul(_in[2].vPosition, g_matWVP);
        vertex.vUV = _in[2].vUV;
        vertex.vViewPos = _in[2].vViewPos;
        vertex.vViewTangent = _in[2].vViewTangent;
        vertex.vViewNormal = _in[2].vViewNormal;
        vertex.vViewBinormal = _in[2].vViewBinormal;
        _outstream.Append(vertex);
    }
    else
        return;
}


// Rasterizer

struct PS_OUT
{
    float4 vColor : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vData : SV_Target3;
    float4 vEmissive : SV_Target4;
};

PS_OUT PS_worldmap_Deferred(GS_OUT _in) : SV_Target
{
    float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    
    float3 vViewNormal = _in.vViewNormal;
     
    // 텍스쳐가 있으면, 해당 색상을 사용한다.
    if (g_btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
       
    // NomalMap 이 있다면
    if (g_btex_1)
    {
        float3x3 matRot =
        {
            _in.vViewTangent,
            -_in.vViewBinormal,
            _in.vViewNormal,
        };

        float3 vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        vNormal = normalize((vNormal * 2.f) - 1.f);
        vViewNormal = mul(vNormal, matRot);
    }
    
    PS_OUT output = (PS_OUT) 0.f;
    
    if (vObjectColor.a <= 0.9f)
    {
        discard;
    }
  
    output.vColor = float4(vObjectColor.xyz, 1.f);
    output.vNormal = float4(vViewNormal.xyz, 1.f);
    output.vPosition = float4(_in.vViewPos.xyz, 1.f);
    
    // 모든 deferred object는 자신의 layer번호를 datattex의 x자리에 집어넣음
    // decal.fx에서 datatex를 가져와 원하는 layer에만 출력시키게 한다.
    if (g_float_0)
    {
        //g_float_0 : 1~32
        //layer는 0~31이지만 한칸 옮겼다.
        float3 data = float3(0.03f * g_float_0, 0.f, 0.f);
        output.vData = float4(data, 1.f);
    }
    else
    {
        output.vData = float4(0.f, 0.f, 0.f, 1.f);
    }
    output.vEmissive = float4(0.f, 0.f, 0.f, 1.f);
        
   return output;
   

}
#endif
