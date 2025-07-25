#ifndef _DECAL
#define _DECAL


#include "value.fx"

// ==========================
// Decal Shader
// RS_TYPE  : CULL_FRONT
// DS_TYPE  : NO_TEST_NO_WRITE
// BS_TYPE  : ONE_ONE

// g_tex_0  : Output Texture
// g_tex_1  : Position Target
// g_tex_2  : Data Target
// g_vec4_0 : LightColor
// g_int_0  : LightUse
// g_int_1  : layerCheck
// g_float_2 : alpha
// =========================
struct VS_DECAL_IN
{
    float3 vPos : POSITION;
};

struct VS_DECAL_OUT
{
    float4 vPosition : SV_Position;
    float3 vViewPos : POSITION;
    float3 vViewNormal : NORMAL;
};

VS_DECAL_OUT VS_Decal(VS_DECAL_IN _in)
{
    VS_DECAL_OUT output = (VS_DECAL_OUT) 0.f;    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_Target0;   
    float4 vEmissive : SV_Target1;
};

PS_OUT PS_Decal(VS_DECAL_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float2 vUV = _in.vPosition.xy / g_Resolution;
    
    float3 vViewPos = g_tex_1.Sample(g_sam_0, vUV).xyz;
    if (!any(vViewPos))
        discard;
    
    float3 vDataViewPos = g_tex_2.Sample(g_sam_0, vUV).xyz;
    //g_int_1 = 0~31
    //datalayer = 1~32
    //datatexture의 r값을 1~32가 아니라 3~96으로 3의 배수로 넣었음
    //0~31로 맞추기 위해 마지막에 1을 뺌
    int DataLayer = vDataViewPos.x * 100 / 3 - 1;
    if (!(g_int_1 & (1 << DataLayer)))
        discard;
    
    
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv);
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv);
    
    if (abs(vLocalPos.x) <= 0.5f
        && abs(vLocalPos.y) <= 0.5f
        && abs(vLocalPos.z) <= 0.5f)
    {
        if(g_btex_0)
        {
            float2 vSampleUV = float2(vLocalPos.x + 0.5f, 0.5f - vLocalPos.z);
            float4 vSampleColor = g_tex_0.Sample(g_sam_0, vSampleUV);
            
            if(g_int_0)
            {
                output.vEmissive = vSampleColor;
            }
            else
            {
                output.vColor = vSampleColor;
            }
        }
        else
        {
            output.vColor = float4(1.f, 0.f, 0.f, 1.f);
        }
    }
    else
    {
        //output.vColor = float4(0.f, 0.f, 1.f, 1.f);
        discard;
    }   
   
    if (g_float_2)
    {
        output.vColor.a *= g_float_2;
    }
    return output;
}




#endif
