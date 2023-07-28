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

// Animation ����
#define bAnimUse                    g_int_0    // Anim Use
#define LeftTop                     g_vec2_0   // AnimAtlas LeftTop
#define Slice                       g_vec2_1   // AnimAtlas Slice
#define Offset                      g_vec2_2
#define BackSize                    g_vec2_3

// MeshRender ���� Material���� �޴� ��
#define IsOutputTextureExist        g_btex_0
#define Output_Texture              g_tex_0
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

    // Sample Texture�� ���� ���
    // g_btex_0 ���� �� 0, 1 �� �ٲ� �� �ִ� �� �˱� ���ؼ� 
    // CMaterial::UpdateData() ����
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
                // UV�� �ʰ��� �ٱ� �κ��� ������. (BackSize����)
                discard;
            }

            // ���İ��� 0�� �κ��� ������
            if (vOutColor.w == 0.f)
                discard;
        }

       else
       {
           // �����ؾ��ϴ� UV���� Offset
           float2 Offset = float2(0.f, 0.f);

           // Output Texture�� �������� �ϴ��� Ȯ��
           if (OutputTexMovingStyle != 0)
           {
               Offset = OutputTexPreviousPos;
           }

           vOutColor = Output_Texture.Sample(g_sam_0, _in.vUV + Offset);

           if (vOutColor.w == 0.f)
               discard;
       }
    }


    // ���۶ձ� (���İ� ó��) 
    if (isPunctureTextureUsed)
    {
        // �����ؾ��ϴ� UV���� Offset
        float2 Offset = float2(0.f, 0.f);

        // Puncture Texture �� �������� �Ǵ��� Ȯ�� 
        if (PunctureTexMovingStyle != 0)
        {
            Offset = PunctureTexPreviousPos;
        }

        float4 vPunctureSample = g_puncture_tex.Sample(g_sam_0, _in.vUV + Offset);

        vOutColor = float4(vOutColor.xyz, vPunctureSample.x);
    }


    // ���� ÷�� (Color Additive)
    if (isAdditiveTextureUsed)
    {
        float4 vAdditiveSample = g_additive_tex.Sample(g_sam_0, _in.vUV);

        if (vAdditiveSample.w != 0)
        {
            vOutColor = float4(vOutColor.x + saturate(AdditiveColor.x) * vAdditiveSample.x * vOutColor.w * vAdditiveSample.w,
                                vOutColor.y + saturate(AdditiveColor.y) * vAdditiveSample.y * vOutColor.w * vAdditiveSample.w,
                                vOutColor.z + saturate(AdditiveColor.z) * vAdditiveSample.z * vOutColor.w * vAdditiveSample.w,
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
        // Normal �� ����
        vNormal = g_tex_1.Sample(g_sam_0, _in.vUV);
        
        // 0 ~ 1 ������ -1 ~ 1 �� ����
        vNormal = (vNormal * 2.f) - 1.f;
        
        // NormalTexture ��ǥ��� y��� z ���� �ݴ�� �Ǿ��ִ�.
        float f = vNormal.y;
        vNormal.y = vNormal.z;
        vNormal.z = f;
        
        // Texture ���� ������ Normal ������ ����� ��ȯ��Ų��.
        vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld)).xyz;
    }
    
    
    
    if (0.f == vOutColor.a)
    {
        return float4(1.f, 0.f, 1.f, 1.f);
        //discard;
    }
     
    
    // Lighting ó��
    tLightColor LightColor = (tLightColor) 0.f;
    
    // vNormal �� Zero Vector ���
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
// g_int_0              : AnimUse (�ִϸ��̼� ��� ����)
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
       
    if (g_btex_0)       // �ؽ�ó ���ε��� �� ���
    {
        // �ִϸ��̼� ����
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
                // UV�� �ʰ��� �ٱ� �κ��� ������.
                //vOutColor = float4(1.f, 1.f, 0.f, 1.f);
                discard;
            }
        }
        // �ִϸ��̼� ������� ���� ��
        else
        {
            // ���ε��� �ؽ�ó ������
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        }
    }
    else // ���ε��� �ؽ�ó�� ���� ��� ����Ÿ�� ���
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    return vOutColor;
}



// ============================
// CoolDownShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_int_0              : AnimUse
// g_vec2_0             : AnimAtlas LeftTop
// g_vec2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// ============================

struct VS_CoolDownIN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_CoolDownOUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_CoolDownOUT VS_CoolDown(VS_CoolDownIN _in)
{
    VS_CoolDownOUT output = (VS_CoolDownOUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_CoolDown(VS_CoolDownOUT _in) : SV_Target
{
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    float2 vYPos = float2(0.5f, 0.f);
    float2 vCenterUV = float2(0.5, 0.5);
    float2 vCurUV = _in.vUV;
    float2 vDir = normalize(vCurUV - vCenterUV);
    float2 vYDir = normalize(vYPos - vCenterUV);

    float angleY = degrees(acos(dot(vDir, vYDir)));

    // ��Ÿ���� ������ �ݴ�� ����մϴ�. (360.f - (360.f * g_float_0))�� ��Ÿ���� ���� á�� �� 0������ �����ؼ� �ð�������� �����ϴ� ���¸� ����ϴ�.
    float angle = 360.f - (360.f * g_float_0);

    if (_in.vUV.x < 0.5)
    {
        angleY = 360.f - angleY;
    }

    if (g_float_0 != 0.f) // g_float_0�� 0�̸� ��Ÿ���� ���� ����, �� �Ϲ� �ؽ��� ���
    {
        if (angleY > angle)
        {
            // Modify the output color to blue for the cooling down part.
            vOutColor = float4(0.06f, 0.23f, 0.34f, 0.6f);
        }
        else
        {
            // Modify the output color to darker for the rest of the texture.
            vOutColor = float4(vOutColor.r * 0.2, vOutColor.g * 0.2, vOutColor.b * 0.2, vOutColor.a);
        }
    }
    else
    {
        // g_float_0�� 0�̸� ��Ÿ���� �����Ƿ� ���� �ؽ��� �״�� ���
        vOutColor = vOutColor;
    }

    return vOutColor;
}

// ============================
// EXPRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            : XP Ratio (����ġ ����)
// ============================

VS_OUT VS_EXPRatio(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV ������ ����

    return output;
}

float4 PS_EXPRatio(VS_OUT _in) : SV_Target
{
    // ����ġ �������� ���� �κ��� ���������� �ʽ��ϴ�.
    if (_in.vUV.y < (1.0f - g_float_0))
    {
        discard;
    }
    // �� �� �κ��� �ؽ��� ���ø��� ���� ������ �����մϴ�.
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}


// ============================
// HPMPRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            :  Ratio (����orü�� ����)
// ============================

VS_OUT VS_HPMPRatio(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV ������ ����

    return output;
}

float4 PS_HPMPRatio(VS_OUT _in) : SV_Target
{
    // ����ġ �������� ���� �κ��� ���������� �ʽ��ϴ�.
    if (_in.vUV.x > g_float_0)
    {
        discard;
    }
    // �� �� �κ��� �ؽ��� ���ø��� ���� ������ �����մϴ�.
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}

// ============================
// WorldBarShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            :  HPRatio (����orü�� ����)
// g_float_1            :  MPRatio (����orü�� ����)
// ============================

VS_OUT VS_WorldBar(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV ������ ����

    return output;
}

float4 PS_WorldBar(VS_OUT _in) : SV_Target
{
    // Define UV boundaries
    
    float BarLeftX = 26.f;
    float BarRightX = 132.f;
    float HPUpY = 5.f;
    float HPDownY = 18.f;
    float MPUpY = 19.f;
    float MPDownY = 24.f;
    float BarTotalX = 136.f;
    float BarTotalY = 29.f;
    
    float2 HPuv_1 = float2(BarLeftX / BarTotalX, HPUpY / BarTotalY);
    float2 HPuv_2 = float2(BarRightX / BarTotalX, HPUpY / BarTotalY);
    float2 HPuv_3 = float2(BarLeftX / BarTotalX, HPDownY / BarTotalY);
    float2 HPuv_4 = float2(BarRightX / BarTotalX, HPDownY / BarTotalY);

    float2 MPuv_1 = float2(BarLeftX / BarTotalX, MPUpY / BarTotalY);
    float2 MPuv_2 = float2(BarRightX / BarTotalX, MPUpY / BarTotalY);
    float2 MPuv_3 = float2(BarLeftX / BarTotalX, MPDownY / BarTotalY);
    float2 MPuv_4 = float2(BarRightX / BarTotalX, MPDownY / BarTotalY);

    // Define HP and MP ratios
    float HP_ratio = g_float_0;
    float MP_ratio = g_float_1;

    // Normalize the UV.x with respect to the HP and MP bar widths
    float normalized_HP_UV_x = (_in.vUV.x - HPuv_1.x) / (HPuv_2.x - HPuv_1.x);
    float normalized_MP_UV_x = (_in.vUV.x - MPuv_1.x) / (MPuv_2.x - MPuv_1.x);

    // Check if the UV is within HP or MP bar and beyond the remaining HP or MP
    if ((_in.vUV.y >= HPuv_1.y && _in.vUV.y <= HPuv_3.y) && _in.vUV.x <= HPuv_2.x && normalized_HP_UV_x > HP_ratio)
    {
        discard;
    }
    else if ((_in.vUV.y >= MPuv_1.y && _in.vUV.y <= MPuv_3.y) && _in.vUV.x <= MPuv_2.x && normalized_MP_UV_x > MP_ratio)
    {
        discard;
    }
    
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}
//VS_OUT VS_WorldBar(VS_IN _in)
//{
//    VS_OUT output = (VS_OUT) 0.f;
    
//    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
//    output.vUV = _in.vUV;
        
//    return output;
//}


//float4 PS_WorldBar(VS_OUT _in) : SV_Target
//{
//    float4 vOutColor = (float4) 0.f;
        
//    if (g_btex_0)
//    {
//        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
//    }
//    else
//    {
//        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
//    }
        
//    return vOutColor;
//}



#endif