#ifndef _TEST
#define _TEST

#include "value.fx"


// VS 입력 구조체
struct VS_IN
{   
    float3 vPos     : POSITION; // semantic  
    float4 vColor   : COLOR;    
    float2 vUV      : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vOutColor : COLOR;
    float2 vOutUV    : TEXCOORD;
};

// vertex shader
// LocalSpace 물체를 NDC 좌표계로 이동
VS_OUT VS_Test(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // 입력으로 들어온 정점좌표에 상수버퍼 값을 더해서 출력    
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
       
    output.vPosition = vProjPos;
    output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;
    
    return output;
}

// pixel shader
float4 PS_Test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;    
  
    if(g_int_0 == 0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV);      
    else if(g_int_0 == 1)
        vColor = g_tex_0.Sample(g_sam_1, _in.vOutUV);       
    
    return vColor;
}



// ===============
// SetColor Shader
// mesh : RectMesh
// ===============
VS_OUT VS_SetColor(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.5f, 1.f);        
    output.vOutUV = _in.vUV;
    
    return output;
}

float4 PS_SetColor(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;   
    
    float fDist = abs(distance(_in.vOutUV, float2(0.5f, 0.5f)));
    
    if (0.5f < fDist)
        discard;
    
    vColor = float4(0.f, 1.f, 0.f, 1.f);
    
    return vColor;
}

#endif