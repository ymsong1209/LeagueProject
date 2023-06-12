#ifndef _LANDSCAPE
#define _LANDSCAPE

#include "value.fx"
#include "func.fx"
#include "struct.fx"


// ========================
// LandScape
#define FaceX           g_int_0
#define FaceZ           g_int_1
#define ColorTexture    g_tex_0
#define NormalTexture   g_tex_1
#define HeightMap       g_tex_2
// ========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_LandScape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vLocalPos = _in.vPos;
    output.vUV = _in.vUV;
      
    return output;
}


// ===========
// Hull Shader
// ===========
// Patch Constant Function
// 패치의 분할 횟수를 지정하는 함수
struct PatchOutput
{
    float Edges[3] : SV_TessFactor;
    float Inside : SV_InsideTessFactor;
};

PatchOutput PatchConstFunc(InputPatch<VS_OUT, 3> _input
    , uint PatchID : SV_PrimitiveID)
{
    PatchOutput output = (PatchOutput)0.f;

    output.Edges[0] = 8;
    output.Edges[1] = 8;
    output.Edges[2] = 8;
    output.Inside = 8;

    return output;
}


struct HS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
//[partitioning("integer")]   // 정수, 실수
[partitioning("fractional_odd")] // 정수, 실수
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstFunc")]
[maxtessfactor(32)]
HS_OUT HS_LandScape(InputPatch<VS_OUT, 3> _input
    , uint i : SV_OutputControlPointID
    , uint PatchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT)0.f;

    output.vLocalPos = _input[i].vLocalPos;
    output.vUV = _input[i].vUV;

    return output;
}



struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


[domain("tri")]
DS_OUT DS_LandScape(const OutputPatch<HS_OUT, 3> _origin
    , float3 _vWeight : SV_DomainLocation
    , PatchOutput _patchtess)
{
    DS_OUT output = (DS_OUT)0.f;

    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;

    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _origin[i].vLocalPos * _vWeight[i];
        vUV += _origin[i].vUV * _vWeight[i];
    }

    // 높이맵 적용
    float2 vHeightMapUV = vUV / float2(FaceX, FaceZ);
    vLocalPos.y = HeightMap.SampleLevel(g_sam_0, vHeightMapUV, 0).x;

    // 투영좌표계
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;

    return output;
}

float4 PS_LandScape(DS_OUT _in) : SV_Target
{
    float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);

    vOutColor = float4(0.f, 1.f, 0.f, 1.f);

    return vOutColor;
}


#endif
