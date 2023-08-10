#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "struct.fx"
#include "func.fx"


// ========================
// Std3D Shader
//
// g_btex_0 : Color Texture가 있는지 판별
// g_btex_1 : Normal Texture가 있는지 판별
// g_tex_0 : Color Texture
// g_tex_1 : Normal Texture
// 
// // Ray Tex용 숫자
#define RayTest                     g_int_2

// MeshRender관련
#define TexMove                     g_int_1
#define OutputTexMoveOffset         g_vec2_4
#define PunctureTexMoveOffset       g_vec2_5
#define IsOutputTextureExist        g_btex_0
#define IsAdditiveTextureExist      g_btex_2
#define IsPunctureTextureExist      g_btex_3
#define Additive_Texture            g_tex_2
#define Puncture_Texture            g_tex_3
#define Additive_Color              g_vec4_0


#define IsOutputTextureExist        g_btex_0
#define Output_Texture              g_tex_0
// ========================


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
   
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL; 
    float3 vBinormal : BINORMAL;
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

VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;    
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);          
    output.vUV = _in.vUV;
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV);
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV));
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV));    
    
    return output;
}

// Rasterizer

float4 PS_Std3D(VS_OUT _in) : SV_Target
{        
    //float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);    
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    
    float3 vViewNormal = _in.vViewNormal;

     
    // 텍스쳐가 있으면, 해당 색상을 사용한다.
    if(g_btex_0)
    {
        //vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        
       
            // 참조해야하는 UV값의 Offset
        float2 Offset = float2(0.f, 0.f);

            // Output Texture가 움직여야 하는지 확인
        if (OutputTexMovingStyle != 0)
        {
            Offset = OutputTexPreviousPos;
        }

        vOutColor = Output_Texture.Sample(g_sam_0, _in.vUV + Offset);

        //if (vOutColor.w == 0.f)
        //    discard;
    }
       
    // NomalMap 이 있다면
    if(g_btex_1)
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
    
   
    //if (isPunctureTextureUsed)
    //{
    //    // 참조해야하는 UV값의 Offset
    //    float2 Offset = float2(0.f, 0.f);

    //    // Puncture Texture 가 움직여야 되는지 확인 
    //    if (PunctureTexMovingStyle != 0)
    //    {
    //        Offset = PunctureTexPreviousPos;
    //    }

    //    float4 vPunctureSample = g_puncture_tex.Sample(g_sam_0, _in.vUV + Offset);

    //    vOutColor = float4(vOutColor.xyz, vPunctureSample.x);
    //}


    //// 색상 첨가 (Color Additive)
    //if (isAdditiveTextureUsed)
    //{
    //    float4 vAdditiveSample = g_additive_tex.Sample(g_sam_0, _in.vUV);

    //    if (vAdditiveSample.w != 0)
    //    {
    //        vOutColor = float4(vOutColor.x + saturate(AdditiveColor.x) * vAdditiveSample.x * vOutColor.w * vAdditiveSample.w,
    //            vOutColor.y + saturate(AdditiveColor.y) * vAdditiveSample.y * vOutColor.w * vAdditiveSample.w,
    //            vOutColor.z + saturate(AdditiveColor.z) * vAdditiveSample.z * vOutColor.w * vAdditiveSample.w,
    //            vOutColor.w);
    //    }
    //}
    
    
    return vOutColor;
}



#endif
