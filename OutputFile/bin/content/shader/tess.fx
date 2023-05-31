#ifndef _TESS
#define _TEXX

#include "value.fx"

// ========================
// TessShader
// g_tex_0 : Color Texture
// ========================
struct VS_INOUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;   
};

// ==============
// Vertex Shader
// ==============
VS_INOUT VS_Tess(VS_INOUT _in)
{
    VS_INOUT output = (VS_INOUT) 0.f;
       
    output = _in;
    
    return output;
}


// ===========
// Hull Shader
// ===========
// Patch Constant Function
// 패치의 분할 횟수를 지정하는 함수
struct PatchOutput
{
    float Edges[3]  : SV_TessFactor;
    float Inside    : SV_InsideTessFactor;
};

PatchOutput PatchConstFunc(InputPatch<VS_INOUT, 3> _input
                            , uint PatchID : SV_PrimitiveID)
{
    PatchOutput output = (PatchOutput) 0.f;
    
    output.Edges[0] = g_float_0;
    output.Edges[1] = g_float_0;
    output.Edges[2] = g_float_0;    
    output.Inside = g_float_0;
    
    return output;
}


struct HS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

[domain("tri")]
//[partitioning("integer")]   // 정수, 실수
[partitioning("fractional_odd")] // 정수, 실수
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstFunc")]
[maxtessfactor(32)]
HS_OUT HS_Tess( InputPatch<VS_INOUT, 3> _input
                , uint i : SV_OutputControlPointID
                , uint PatchID : SV_PrimitiveID)
{
    HS_OUT output = (HS_OUT) 0.f;
    
    output.vPos = _input[i].vPos;    
    output.vUV = _input[i].vUV;
    
    return output;
}



struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


[domain("tri")]
DS_OUT DS_Tess(const OutputPatch<HS_OUT, 3> _origin
                , float3 _vWeight : SV_DomainLocation
                , PatchOutput _patchtess)
{
    DS_OUT output = (DS_OUT) 0.f;        
    
    //float3 vLocalPos = (_origin[0].vPos * _vWeight[0]) + (_origin[1].vPos * _vWeight[1]) + (_origin[2].vPos * _vWeight[2]);
    //float2 vUV = (_origin[0].vUV * _vWeight[0]) + (_origin[1].vUV * _vWeight[1]) + (_origin[2].vUV * _vWeight[2]);
  
    float3 vLocalPos = (float3) 0.f;
    float2 vUV = (float2) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        vLocalPos += _origin[i].vPos * _vWeight[i];
        vUV += _origin[i].vUV * _vWeight[i];
    }    
    
    output.vPosition = mul(float4(vLocalPos, 1.f), g_matWVP);
    output.vUV = vUV;
    
    return output;
}

// ============
// Pixel Shader
// ============
float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 vObjectColor = float4(0.f, 1.f, 0.f, 1.f);
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
   
    vOutColor.xyz = vObjectColor;    
    return vOutColor;
}






#endif

