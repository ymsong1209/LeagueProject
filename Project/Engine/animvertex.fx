#ifndef _ANIMVERTEX
#define _ANIMVERTEX

#include "value.fx"
#include "func.fx"
#include "struct.fx"

// ========================
// AnimVertex Shader
//
// g_tex_0 : Color Texture
// g_tex_1 : Normal Texture
// g_float_0 : LayerNum
// 
// // Ray Tex�� ����
#define RayTest                     g_int_2

// MeshRender����
#define TexMove                     g_int_1
#define OutputTexMoveOffset         g_vec2_4
#define PunctureTexMoveOffset       g_vec2_5
#define IsOutputTextureExist        g_btex_0
#define IsAdditiveTextureExist      g_btex_2
#define IsPunctureTextureExist      g_btex_3
#define Additive_Texture            g_tex_2
#define Puncture_Texture            g_tex_3
#define Additive_Color              g_vec4_0
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

VS_OUT VS_ANIM_VERTEX(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);

    if (g_int_11 == 100)
    {

        float3 AnimLocalPos = float3(g_vec4_6.x, g_vec4_6.y, g_vec4_6.z);

        /* if (g_vec4_6.x >= 23 && g_vec4_6.x <= 26)
         {
             output.vPosition.x += 100;
         }*/


        //AnimLocalPos = TrackSkinning(AnimLocalPos, g_vec4_10, g_vec4_11, 0, g_int_10);
        AnimLocalPos = TrackSkinning(_in.vPos, g_vec4_10, g_vec4_11, 0, g_int_10);

        float3  LocalPos = _in.vPos + AnimLocalPos;
        output.vPosition.x += AnimLocalPos.x;
        output.vPosition.y += AnimLocalPos.y;
        output.vPosition.z += AnimLocalPos.z;



    }

    output.vUV = _in.vUV;

    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));

    return output;
}

struct PS_OUT
{
    float4 vColor       : SV_Target0;
    float4 vNormal      : SV_Target1;
    float4 vPosition    : SV_Target2;
    float4 vData        : SV_Target3;
    float4 vEmissive    : SV_Target4;
};

PS_OUT PS_ANIM_VERTEX(VS_OUT _in) : SV_Target
{
    float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);

    float3 vViewNormal = _in.vViewNormal;

    if (g_int_11 == 100)
    {
        PS_OUT TempOut = (PS_OUT)0.f;

        TempOut.vColor = float4(1.f, 1.f, 0.f, 1.f);
        TempOut.vNormal = float4(vViewNormal.xyz, 1.f);
        TempOut.vPosition = float4(_in.vViewPos.xyz, 1.f);

        return TempOut;
    }


    // �ؽ��İ� ������, �ش� ������ ����Ѵ�.
    if (g_btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }

    // NomalMap �� �ִٸ�
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

    PS_OUT output = (PS_OUT)0.f;

    if (vObjectColor.a <= 0.9f)
    {
        discard;
    }

    output.vColor = float4(vObjectColor.xyz, 1.f);
    output.vNormal = float4(vViewNormal.xyz, 1.f);
    output.vPosition = float4(_in.vViewPos.xyz, 1.f);

    // ��� deferred object�� �ڽ��� layer��ȣ�� datattex�� x�ڸ��� �������
    // decal.fx���� datatex�� ������ ���ϴ� layer���� ��½�Ű�� �Ѵ�.
    if (g_float_0)
    {
        //g_float_0 : 1~32
        //layer�� 0~31������ ��ĭ �Ű��.
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