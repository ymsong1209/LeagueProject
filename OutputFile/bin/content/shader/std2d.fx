#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define BackSize        g_vec2_3

// Parameter
// g_int_0              : AnimUse
// g_int_1              : TexMove
// g_vec2_0             : AnimAtlas LeftTop
// g_vec2_1             : AnimAtlas Slice
//
// g_tex_0              : Output   Texture
// g_tex_1              : Puncture Texture
// g_tex_2              : Additive Texture
// g_vec4_0             : Additive Color
// g_vec2_2             : Output Tex Move Offset
// g_vec2_3             : Puncture Tex Move Offset
#define IsOutputTextureExist        g_btex_0
#define IsPunctureTextureExist      g_btex_1
#define IsAdditiveTextureExist      g_btex_2
#define Output_Texture              g_tex_0 
#define Puncture_Texture            g_tex_1
#define Additive_Texture            g_tex_2
#define Additive_Color              g_vec4_0
// ============================
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Std2D(VS_OUT _in) : SV_Target
{
 
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
       
    // Sample Texture가 없는 경우
    // g_btex_0 값이 왜 0, 1 로 바뀔 수 있는 지 알기 위해선 
    // CMaterial::UpdateData() 참고
    if (IsOutputTextureExist)
    {
        if (bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= ((BackSize - Slice) / 2.f);
            vUV -= Offset;

            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_0, vUV);
            }
            else
            {
                // UV를 초과한 바깥 부분은 버린다.
               // vOutColor = float4(1.f, 1.f, 0.f, 1.f);
                discard;
            }
        }

        else
        {
            // 참조해야하는 UV값의 Offset
            float2 Offset = float2(0.f, 0.f);

            // Output Texture가 움직여야 하는지 확인
            int assist_bit = 1;
            assist_bit = g_int_1 & (assist_bit);

            if (!assist_bit)
            {
                Offset = g_vec2_2;
            }

            vOutColor = Output_Texture.Sample(g_sam_0, _in.vUV + Offset);

            if (vOutColor.w == 0.f)
                discard;
        }
    }


    // 구멍뚫기 (알파값 처리)
    if (IsPunctureTextureExist)
    {
        // 참조해야하는 UV값의 Offset
        float2 Offset = float2(0.f, 0.f);

        // Puncture Texture 가 움직여야 되는지 확인 
        int assist_bit = 2;
        assist_bit = g_int_1 & (assist_bit);

        if (!assist_bit)
        {
            Offset = g_vec2_3;
        }


        float4 vPunctureSample = Puncture_Texture.Sample(g_sam_0, _in.vUV + Offset);

        vOutColor = float4(vOutColor.xyz, vPunctureSample.x);
    }


    // 색상 첨가 (Color Additive)
    if (IsAdditiveTextureExist)
    {
        float4 vAdditiveSample = Additive_Texture.Sample(g_sam_0, _in.vUV);

        if (vAdditiveSample.w != 0)
        {
            vOutColor = float4( vOutColor.x + saturate(Additive_Color.x) * vAdditiveSample.x * vOutColor.w * vAdditiveSample.w,
                                vOutColor.y + saturate(Additive_Color.y) * vAdditiveSample.y * vOutColor.w * vAdditiveSample.w,
                                vOutColor.z + saturate(Additive_Color.z) * vAdditiveSample.z * vOutColor.w * vAdditiveSample.w,
                                vOutColor.w);
        }

       
    }
    
 
    return vOutColor;
}









// ======================================
// Std2DLightShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define BackSize        g_vec2_3

// g_tex_0              : Output Texture
// g_tex_1              : Nomal Texture
// ======================================

struct VS_Light_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Light_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};



VS_Light_OUT VS_Std2DLight(VS_Light_IN _in)
{
    VS_Light_OUT output = (VS_Light_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;
        
    return output;
}


float4 PS_Std2DLight(VS_Light_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_vec4_0.x == 3.14f)
    {
        return float4(1.f, 0.f, 0.f, 1.f);
    }
    
    if (g_btex_0)
    {
        if (bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= ((BackSize - Slice) / 2.f);
            vUV -= Offset;
            
            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_0, vUV);
            }
            else
            {
                vOutColor = float4(1.f, 1.f, 0.f, 1.f);
                //discard;
            }
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
        
    float3 vNormal = (float3) 0.f;
    if (g_btex_1)
    {
        // Normal 값 추출
        vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        
        // 0 ~ 1 범위를 -1 ~ 1 로 변경
        vNormal = (vNormal * 2.f) - 1.f;
        
        // NormalTexture 좌표계는 y축과 z 축이 반대로 되어있다.
        float f = vNormal.y;
        vNormal.y = vNormal.z;
        vNormal.z = f;
        
        // Texture 에서 추출한 Normal 방향을 월드로 변환시킨다.
        vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld)).xyz;
    }
    
    
    
    if (0.f == vOutColor.a)
    {
        return float4(1.f, 0.f, 1.f, 1.f);
        //discard;
    }
     
    
    // Lighting 처리
    tLightColor LightColor = (tLightColor) 0.f;
    
    // vNormal 이 Zero Vector 라면
    if (dot(vNormal, vNormal) == 0.f)
    {
        CalcLight2D(_in.vWorldPos, LightColor);
    }
    else
    {
        CalcLight2D(_in.vWorldPos, vNormal, LightColor);
    }
        
    vOutColor.rgb *= (LightColor.vDiffuse.rgb + LightColor.vAmbient.rgb);
    
    return vOutColor;
}


// ======================================
// Std2DAnimShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_int_0              : AnimUse (애니메이션 사용 여부)
// g_vec2_0             : AnimAtlas LeftTop
// g_vec2_1             : AnimAtlas Slice
// g_vec2_2             : AnimAtlas Offset
// g_vec2_3             : AnimAtlas BackSize

#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define BackSize        g_vec2_3
// ========================================

struct VS_Anim_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Anim_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


VS_Anim_OUT VS_Anim2D(VS_Anim_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.5f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Anim2D(VS_Anim_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_btex_0)       // 텍스처 바인딩이 된 경우
    {
        // 애니메이션 사용시
        if (bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= ((BackSize - Slice) / 2.f);
            vUV -= Offset;
            
            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_0, vUV);
            }
            else
            {
                // UV를 초과한 바깥 부분은 버린다.
                //vOutColor = float4(1.f, 1.f, 0.f, 1.f);
                discard;
            }
        }
        // 애니메이션 사용하지 않을 시
        else
        {
            // 바인딩된 텍스처 렌더링
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    else // 바인딩된 텍스처가 없을 경우 마젠타색 출력
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return vOutColor;
}


#endif