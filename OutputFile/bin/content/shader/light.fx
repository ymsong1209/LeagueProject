#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// ========================
// Directional Light Shader
// mesh : RectMesh
// g_int_0 : Light Index
// g_tex_0 : Position Target
// g_tex_1 : Normal Target
// ========================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_DirLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}


struct PS_OUT
{
    float4 vDiffuse     : SV_Target0;
    float4 vSpecular    : SV_Target1;
};

PS_OUT PS_DirLightShader(VS_OUT _in)
{ 
    PS_OUT output = (PS_OUT) 0.f;
    
    // Position Target 
    float3 vViewPos = g_tex_0.Sample(g_sam_0, _in.vUV).xyz;
    
    if (!any(vViewPos.xyz))
        discard;
            
    // Normal Target
    float3 vViewNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
    
    tLightColor lightcolor = (tLightColor) 0.f;
    CalcLight3D(vViewPos, vViewNormal, g_int_0, lightcolor);
        
    output.vDiffuse = lightcolor.vDiffuse + lightcolor.vAmbient;
    output.vSpecular = lightcolor.vSpecular;    
        
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}


// ========================
// Point Light Shader
// mesh : SphereMesh
// g_int_0 : Light Index
// g_tex_0 : Position Target
// g_tex_1 : Normal Target
// ========================
VS_OUT VS_PointLightShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
        
    return output;
}

PS_OUT PS_PointLightShader(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
      
    float2 vUV = _in.vPosition.xy / g_Resolution;
    
    float3 vViewPos = g_tex_0.Sample(g_sam_0, vUV).xyz;    
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv);
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv);
    
    if (length(vLocalPos) <= 0.5f)
    {
        // ³»ºÎ   
        
        tLightColor lightcolor = (tLightColor) 0.f;
        float3 vViewNormal = g_tex_1.Sample(g_sam_0, vUV).xyz;
        
        CalcLight3D(vViewPos, vViewNormal, g_int_0, lightcolor);        
        
        output.vDiffuse = lightcolor.vDiffuse + lightcolor.vAmbient;
        output.vSpecular = lightcolor.vSpecular;
    }
    else
    {
        discard;
        //output.vDiffuse = float4(0.f, 0.f, 1.f, 1.f);
        //output.vSpecular = float4(0.f, 0.f, 1.f, 1.f);
    }
    
    return output;
}


// ========================
// Light Merge Shader
// mesh : RectMesh
// g_tex_0 : ColorTarget
// g_tex_1 : DiffuseTarget
// g_tex_2 : SpecularTarget
// g_tex_3 : EmissiveTarget
// ========================
VS_OUT VS_LightMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_LightMerge(VS_OUT _in) : SV_Target0
{
    float4 vOutColor = (float4) 0.f;
    
    // Position Target 
    float3 Color = g_tex_0.Sample(g_sam_0, _in.vUV).xyz;
    float3 Diffuse = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
    float3 Specular = g_tex_2.Sample(g_sam_0, _in.vUV).xyz;
    float3 Emissive = g_tex_3.Sample(g_sam_0, _in.vUV).xyz;
    
    vOutColor.xyz = (Color * Diffuse) + Emissive + Specular;
    vOutColor.a = 1.f;
    
    return vOutColor;
}









#endif
