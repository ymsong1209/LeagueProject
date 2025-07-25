#ifndef _DEFERRED
#define _DEFERRED

#include "value.fx"
#include "func.fx"
#include "struct.fx"

// ========================
// Std3D Deferred Shader
//
// g_tex_0 : Color Texture
// g_tex_1 : Normal Texture
// g_float_0 : LayerNum
// 
// g_float_9 : alpha


// MeshRender관련

#define IsOutputTextureExist        g_btex_0
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

struct VTX_IN_INST
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;

    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;

    // Per Instance Data
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint iRowIndex : ROWINDEX;
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

VS_OUT VS_Std3D_Deferred(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));
    
    return output;
}

VS_OUT VS_Std3D_Deferred_Inst(VTX_IN_INST _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, _in.iRowIndex);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
    output.vUV = _in.vUV;

    output.vViewPos = mul(float4(_in.vPos, 1.f), _in.matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), _in.matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), _in.matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), _in.matWV));

    return output;
}


// Rasterizer

struct PS_OUT
{
    float4 vColor       : SV_Target0;
    float4 vNormal      : SV_Target1;
    float4 vPosition    : SV_Target2;
    float4 vData        : SV_Target3;
    float4 vEmissive    : SV_Target4;
};

PS_OUT PS_Std3D_Deferred(VS_OUT _in) : SV_Target
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
    
    if (g_float_9)
    {
        output.vColor.a *= g_float_9;
    }
    
    return output;
}



// ========================
// ContourPaint Shader
// ========================

struct VS_ContourPaint_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
   
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;

    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_ContourPaint_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_ContourPaint_OUT VS_ContourPaint_Deferred(VS_ContourPaint_IN _in)
{
    VS_ContourPaint_OUT output = (VS_ContourPaint_OUT) 0.f;
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    return output;
}
// Rasterizer

struct PS_ContourPaint_OUT
{
    float4 vContour : SV_Target6;
};

PS_ContourPaint_OUT PS_ContourPaint_Deferred(VS_ContourPaint_OUT _in) : SV_Target6
{
    float4 vObjectColor = float4(0.f, 0.f, 0.f, 1.f);
    if (g_btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    if (vObjectColor.a <= 0.f)
    {
        discard;
    }
    
    
    PS_ContourPaint_OUT output = (PS_ContourPaint_OUT) 0.f;
    output.vContour = float4(0.f, 0.f, 1.f, 1.f);
        
    return output;
}



// ========================
// DefaultObjWrite Shader
// ========================

struct VS_DefaultObjWrite_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_DefaultObjWrite_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_DefaultObjWrite_OUT VS_DefaultTexWrite(VS_DefaultObjWrite_IN _in)
{
    VS_DefaultObjWrite_OUT output = (VS_DefaultObjWrite_OUT) 0.f;
    if (g_iAnim)
    {
        Skinning(_in.vPos, _in.vTangent, _in.vBinormal, _in.vNormal, _in.vWeights, _in.vIndices, 0);
    }

    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    return output;
}
// Rasterizer

struct PS_DefaultObjWrite_OUT
{
    float4 vDefaultContour : SV_Target5;
};

PS_DefaultObjWrite_OUT PS_DefaultObjWrite(VS_DefaultObjWrite_OUT _in) : SV_Target5
{
    float4 vObjectColor = float4(0.f, 0.f, 0.f, 1.f);
    if (g_btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    if (vObjectColor.a <= 0.f)
    {
        discard;
    }
    
    PS_DefaultObjWrite_OUT output = (PS_DefaultObjWrite_OUT) 0.f;
    output.vDefaultContour = float4(0.f, 1.f, 0.f, 1.f);
    return output;
}

#endif
