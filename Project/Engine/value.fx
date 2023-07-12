#ifndef _VALUE
#define _VALUE

#include "struct.fx"

cbuffer TRANSFORM : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matWorldInv;
    row_major matrix g_matView;
    row_major matrix g_matViewInv;
    row_major matrix g_matProj;    
    
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
};

cbuffer MATERIAL : register(b1)
{
    float4 g_vDiff;
    float4 g_vSpec;
    float4 g_vAmb;
    float4 g_vEmv;
    
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    int g_int_4;
    int g_int_5;
    int g_int_6;
    int g_int_7;
    int g_int_8;
    int g_int_9;
    int g_int_10;
    int g_int_11;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    float g_float_4;
    float g_float_5;
    float g_float_6;
    float g_float_7;
    float g_float_8;
    float g_float_9;
    float g_float_10;
    float g_float_11;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    float2 g_vec2_4;
    float2 g_vec2_5;
    float2 g_vec2_6;
    float2 g_vec2_7;
    float2 g_vec2_8;
    float2 g_vec2_9;
    float2 g_vec2_10;
    float2 g_vec2_11;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    float4 g_vec4_4;
    float4 g_vec4_5;
    float4 g_vec4_6;
    float4 g_vec4_7;
    float4 g_vec4_8;
    float4 g_vec4_9;
    float4 g_vec4_10;
    float4 g_vec4_11;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;   
    
    int g_bcube_0;
    int g_bcube_1;

    int g_btexarr_0;
    int g_btexarr_1;

    // 3D Animation 정보
    int             g_iAnim;
    int             g_iBoneCount;
    int2            padding;
};

cbuffer GLOBAL : register(b2)
{
    float2 g_Resolution;
    float  g_DT;                //Play상태일때 1프레임당 흐르는 시간
    float  g_AccTime;
  
    uint   g_Light2DCount;
    uint   g_Light3DCount;
    float  g_EditDT;            //Play,Pause상관없이 1프레임당 흐르는 시간
    int    g_globalpadding;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

TextureCube g_cube_0 : register(t8);
TextureCube g_cube_1 : register(t9);

Texture2DArray g_texarr_0 : register(t10);
Texture2DArray g_texarr_1 : register(t11);

StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t12);
StructuredBuffer<tLightInfo> g_Light3DBuffer : register(t13);

// Animation3D Bone Matrix Buffer
StructuredBuffer<Matrix> g_arrBoneMat : register(t30);


SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);


#endif