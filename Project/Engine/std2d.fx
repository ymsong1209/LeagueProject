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

// Animation 관련
#define bAnimUse                    g_int_0    // Anim Use
#define LeftTop                     g_vec2_0   // AnimAtlas LeftTop
#define Slice                       g_vec2_1   // AnimAtlas Slice
#define Offset                      g_vec2_2
#define BackSize                    g_vec2_3

// MeshRender 관련 Material에서 받는 값
#define IsOutputTextureExist        g_btex_0
#define Output_Texture              g_tex_0

// Ray Tex용 숫자
#define RayTest          g_int_2
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
                // UV를 초과한 바깥 부분은 버린다. (BackSize관련)
                discard;
            }

            // 알파값이 0인 부분은 버린다
            if (vOutColor.w == 0.f)
                discard;
        }
        else
        {
            // 참조해야하는 UV값의 Offset
            float2 Offset = float2(0.f, 0.f);

            // Output Texture가 움직여야 하는지 확인
            if (OutputTexMovingStyle != 0)
            {
                Offset = OutputTexPreviousPos;
            }

            vOutColor = Output_Texture.Sample(g_sam_0, _in.vUV + Offset);

            if (vOutColor.w == 0.f)
                discard;
        }
    }

    // 구멍뚫기 (알파값 처리) 
    if (isPunctureTextureUsed)
    {
        // 참조해야하는 UV값의 Offset
        float2 Offset = float2(0.f, 0.f);

        // Puncture Texture 가 움직여야 되는지 확인 
        if (PunctureTexMovingStyle != 0)
        {
            Offset = PunctureTexPreviousPos;
        }

        float4 vPunctureSample = g_puncture_tex.Sample(g_sam_0, _in.vUV + Offset);

        vOutColor = float4(vOutColor.xyz, vPunctureSample.x);
    }


    // 색상 첨가 (Color Additive)
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
    VS_Anim_OUT output = (VS_Anim_OUT) 0.f;
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

    // 쿨타임의 각도를 반대로 계산합니다. (360.f - (360.f * g_float_0))는 쿨타임이 가득 찼을 때 0도부터 시작해서 시계방향으로 감소하는 형태를 만듭니다.
    float angle = 360.f - (360.f * g_float_0);

    if (_in.vUV.x < 0.5)
    {
        angleY = 360.f - angleY;
    }

    if (g_float_0 != 0.f) // g_float_0이 0이면 쿨타임이 없는 상태, 즉 일반 텍스쳐 출력
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
        // g_float_0이 0이면 쿨타임이 없으므로 원래 텍스쳐 그대로 출력
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
// g_float_0            : XP Ratio (경험치 비율)
// ============================

VS_OUT VS_EXPRatio(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_EXPRatio(VS_OUT _in) : SV_Target
{
    // 경험치 비율보다 낮은 부분은 렌더링하지 않습니다.
    if (_in.vUV.y < (1.0f - g_float_0))
    {
        discard;
    }
    // 그 외 부분은 텍스쳐 샘플링을 통해 색상을 결정합니다.
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}


// ============================
// HPMPRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            :  Ratio (마나or체력 비율)
// ============================

VS_OUT VS_HPMPRatio(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_HPMPRatio(VS_OUT _in) : SV_Target
{
    // 경험치 비율보다 낮은 부분은 렌더링하지 않습니다.
    if (_in.vUV.x > g_float_0)
    {
        discard;
    }
    // 그 외 부분은 텍스쳐 샘플링을 통해 색상을 결정합니다.
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}

// ============================
// 2dDefaultShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            :  Ratio (마나or체력 비율)
// ============================

VS_OUT VS_2dDefaultShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_2dDefaultShader(VS_OUT _in) : SV_Target
{
    // 그 외 부분은 텍스쳐 샘플링을 통해 색상을 결정합니다.
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    return vOutColor;
}

// ============================
// WorldBarShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            :  HPRatio (마나or체력 비율)
// g_float_1            :  MPRatio (마나or체력 비율)
// ============================

VS_OUT VS_WorldBar(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_WorldBar(VS_OUT _in) : SV_Target
{
    // Define UV boundaries
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float BarLeftX = 26.f;
    float BarRightX = 129.7f;
    float HPUpY = 5.f;
    float HPDownY = 18.f;
    float MPUpY = 19.f;
    float MPDownY = 24.f;
    float BarTotalX = 133.f;
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
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }
    else if ((_in.vUV.y >= MPuv_1.y && _in.vUV.y <= MPuv_3.y) && _in.vUV.x <= MPuv_2.x && normalized_MP_UV_x > MP_ratio)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }
    

    return vOutColor;
}


// ============================
// BarRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            : 현재 bar대비 비율 (마나or체력 비율)
// g_float_1            : 체력바의 왼쪽 x지점
// g_float_2            : 체력바의 오른쪽 x지점
// g_float_3            : 체력바의 위 y지점
// g_float_4            : 체력바의 아래 y지점
// g_float_5            : 전체 텍스쳐 가로
// g_float_6            : 전체 텍스쳐 세로

// Animation 관련
#define Ratio                   g_float_0  
#define LeftX                   g_float_1  
#define RightX                  g_float_2  
#define UpY                     g_float_3  
#define DownY                   g_float_4
#define TotalWidth              g_float_5
#define TotalHeight             g_float_6
// ============================

VS_OUT VS_BarRatioShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_BarRatioShader(VS_OUT _in) : SV_Target
{
    // Define UV boundaries
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float2 HPuv_1 = float2(LeftX / TotalWidth, UpY / TotalHeight);
    float2 HPuv_2 = float2(RightX / TotalWidth, UpY / TotalHeight);
    float2 HPuv_3 = float2(LeftX / TotalWidth, DownY / TotalHeight);
    float2 HPuv_4 = float2(RightX / TotalWidth, DownY / TotalHeight);

    // Define HP and MP ratios
    float HP_ratio = g_float_0;

    // Normalize the UV.x with respect to the HP and MP bar widths
    float normalized_HP_UV_x = (_in.vUV.x - HPuv_1.x) / (HPuv_2.x - HPuv_1.x);

    // Check if the UV is within HP or MP bar and beyond the remaining HP or MP
    if ((_in.vUV.y >= HPuv_1.y && _in.vUV.y <= HPuv_3.y) && _in.vUV.x <= HPuv_2.x && normalized_HP_UV_x > HP_ratio)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }

    return vOutColor;
}

// ============================
// JungleBarRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            : 현재 bar대비 비율 (마나or체력 비율)
// ============================

VS_OUT VS_JungleBarRatioShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_JungleBarRatioShader(VS_OUT _in) : SV_Target
{
    float totalWidth = 151;
    float totalHeight = 43;
    float leftx = 4;
    float Rightx = 147.2;
    float upY = 21;
    float downY = 33;
    // Define UV boundaries
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float2 HPuv_1 = float2(leftx / totalWidth, upY / totalHeight);
    float2 HPuv_2 = float2(Rightx / totalWidth, upY / totalHeight);
    float2 HPuv_3 = float2(leftx / totalWidth, downY / totalHeight);
    float2 HPuv_4 = float2(Rightx / totalWidth, downY / totalHeight);

    // Define HP and MP ratios
    float HP_ratio = g_float_0;

    // Normalize the UV.x with respect to the HP and MP bar widths
    float normalized_HP_UV_x = (_in.vUV.x - HPuv_1.x) / (HPuv_2.x - HPuv_1.x);

    // Check if the UV is within HP or MP bar and beyond the remaining HP or MP
    if ((_in.vUV.y >= HPuv_1.y && _in.vUV.y <= HPuv_3.y) && _in.vUV.x <= HPuv_2.x && normalized_HP_UV_x > HP_ratio)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }

    return vOutColor;
}



// ============================
// MinionBarRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            : 현재 bar대비 비율 (마나or체력 비율)
// ============================

VS_OUT VS_MinionBarRatioShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_MinionBarRatioShader(VS_OUT _in) : SV_Target
{
    float totalWidth = 66.f;
    float totalHeight = 7.f;
    float leftx = 0.8f;
    float Rightx = 65;
    float upY = 0.7f;
    float downY = 6.5f;
    // Define UV boundaries
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float2 HPuv_1 = float2(leftx / totalWidth, upY / totalHeight);
    float2 HPuv_2 = float2(Rightx / totalWidth, upY / totalHeight);
    float2 HPuv_3 = float2(leftx / totalWidth, downY / totalHeight);
    float2 HPuv_4 = float2(Rightx / totalWidth, downY / totalHeight);

    // Define HP and MP ratios
    //float HP_ratio = g_float_0;
    float HP_ratio = g_float_0;

    // Normalize the UV.x with respect to the HP and MP bar widths
    float normalized_HP_UV_x = (_in.vUV.x - HPuv_1.x) / (HPuv_2.x - HPuv_1.x);

    // Check if the UV is within HP or MP bar and beyond the remaining HP or MP
    if ((_in.vUV.y >= HPuv_1.y && _in.vUV.y <= HPuv_3.y) && _in.vUV.x <= HPuv_2.x && normalized_HP_UV_x > HP_ratio)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }

    return vOutColor;
}


// ============================
// TurretBarRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            : 현재 bar대비 비율 (마나or체력 비율)
// ============================

VS_OUT VS_TurretBarRatioShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_TurretBarRatioShader(VS_OUT _in) : SV_Target
{
    float totalWidth = 192.f;
    float totalHeight = 35.f;
    float leftx = 11.f;
    float Rightx = 180.f;
    float upY = 7.f;
    float downY = 21.f;
    // Define UV boundaries
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float2 HPuv_1 = float2(leftx / totalWidth, upY / totalHeight);
    float2 HPuv_2 = float2(Rightx / totalWidth, upY / totalHeight);
    float2 HPuv_3 = float2(leftx / totalWidth, downY / totalHeight);
    float2 HPuv_4 = float2(Rightx / totalWidth, downY / totalHeight);

    // Define HP and MP ratios
    //float HP_ratio = g_float_0;
    float HP_ratio = g_float_0;

    // Normalize the UV.x with respect to the HP and MP bar widths
    float normalized_HP_UV_x = (_in.vUV.x - HPuv_1.x) / (HPuv_2.x - HPuv_1.x);

    // Check if the UV is within HP or MP bar and beyond the remaining HP or MP
    if ((_in.vUV.y >= HPuv_1.y && _in.vUV.y <= HPuv_3.y) && _in.vUV.x <= HPuv_2.x && normalized_HP_UV_x > HP_ratio)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }

    return vOutColor;
}



// ============================
// InhibitorBarRatioShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_float_0            : 현재 bar대비 비율 (마나or체력 비율)
// ============================

VS_OUT VS_InhibitorBarRatioShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_InhibitorBarRatioShader(VS_OUT _in) : SV_Target
{
    float totalWidth = 151.f;
    float totalHeight = 23.f;
    float leftx = 3.f;
    float Rightx = 147.f;
    float upY = 3.f;
    float downY = 14.f;
    // Define UV boundaries
    // Sample the color from the texture
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float2 HPuv_1 = float2(leftx / totalWidth, upY / totalHeight);
    float2 HPuv_2 = float2(Rightx / totalWidth, upY / totalHeight);
    float2 HPuv_3 = float2(leftx / totalWidth, downY / totalHeight);
    float2 HPuv_4 = float2(Rightx / totalWidth, downY / totalHeight);

    // Define HP and MP ratios
    //float HP_ratio = g_float_0;
    float HP_ratio = g_float_0;

    // Normalize the UV.x with respect to the HP and MP bar widths
    float normalized_HP_UV_x = (_in.vUV.x - HPuv_1.x) / (HPuv_2.x - HPuv_1.x);

    // Check if the UV is within HP or MP bar and beyond the remaining HP or MP
    if ((_in.vUV.y >= HPuv_1.y && _in.vUV.y <= HPuv_3.y) && _in.vUV.x <= HPuv_2.x && normalized_HP_UV_x > HP_ratio)
    {
        vOutColor = float4(0.f, 0.f, 0.f, 0.75f);
    }

    return vOutColor;
}



// ============================
// SkillLevelShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_int_0              :스킬Q 레벨
// g_int_1              :스킬W 레벨
// g_int_2              :스킬E 레벨
// g_int_3              :스킬R 레벨

#define Q_Level          g_int_0
#define W_Level          g_int_1
#define E_Level          g_int_2
#define R_Level          g_int_3
// ============================

VS_OUT VS_SkillLevelShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_SkillLevelShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float2 TexSize = float2(279.f, 9.f);
    
    float2 Skill_Q_discardRegionMin = float2(0.f, 0.f) / TexSize;
    float2 Skill_Q_discardRegionMax = float2(56.f, 9.f) / TexSize;
    
    if (Q_Level == 0)
        Skill_Q_discardRegionMin = float2(0.f, 0.f) / TexSize;
    else if (Q_Level == 1)
        Skill_Q_discardRegionMin = float2(10.f, 0.f) / TexSize;
    else if (Q_Level == 2)
        Skill_Q_discardRegionMin = float2(21.f, 0.f) / TexSize;
    else if (Q_Level == 3)
        Skill_Q_discardRegionMin = float2(32.f, 0.f) / TexSize;
    else if (Q_Level == 4)
        Skill_Q_discardRegionMin = float2(43.f, 0.f) / TexSize;
    else if (Q_Level == 5)
        Skill_Q_discardRegionMin = float2(56.f, 0.f) / TexSize;
    
    //==================W=======================================
    
    float2 Skill_W_discardRegionMin = float2(0.f, 0.f) / TexSize;
    float2 Skill_W_discardRegionMax = float2(132.f, 9.f) / TexSize;
    
    if (W_Level == 0)
        Skill_W_discardRegionMin = float2(74.f, 0.f) / TexSize;
    else if (W_Level == 1)
        Skill_W_discardRegionMin = float2(86.f, 0.f) / TexSize;
    else if (W_Level == 2)
        Skill_W_discardRegionMin = float2(97.f, 0.f) / TexSize;
    else if (W_Level == 3)
        Skill_W_discardRegionMin = float2(108.f, 0.f) / TexSize;
    else if (W_Level == 4)
        Skill_W_discardRegionMin = float2(119.f, 0.f) / TexSize;
    else if (W_Level == 5)
        Skill_W_discardRegionMin = float2(132.f, 0.f) / TexSize;

    
        // 텍스쳐의 특정 영역
    float2 Skill_E_discardRegionMin = float2(0.f, 0.f) / TexSize;
    float2 Skill_E_discardRegionMax = float2(207.f, 9.f) / TexSize;
    
    if (E_Level == 0)
        Skill_E_discardRegionMin = float2(149.f, 0.f) / TexSize;
    else if (E_Level == 1)
        Skill_E_discardRegionMin = float2(161.f, 0.f) / TexSize;
    else if (E_Level == 2)
        Skill_E_discardRegionMin = float2(172.f, 0.f) / TexSize;
    else if (E_Level == 3)
        Skill_E_discardRegionMin = float2(183.f, 0.f) / TexSize;
    else if (E_Level == 4)
        Skill_E_discardRegionMin = float2(194.f, 0.f) / TexSize;
    else if (E_Level == 5)
        Skill_E_discardRegionMin = float2(207.f, 0.f) / TexSize;
    
    
        // 텍스쳐의 특정 영역
    float2 Skill_R_discardRegionMin = float2(0.f, 0.f) / TexSize;
    float2 Skill_R_discardRegionMax = float2(279.f, 9.f) / TexSize;
    
    //if (R_Level == 0)
    //    Skill_R_discardRegionMin = float2(223.f, 0.f) / TexSize;
    if (R_Level == 0)
        Skill_R_discardRegionMin = float2(236.f, 0.f) / TexSize;
    else if (R_Level == 1)
        Skill_R_discardRegionMin = float2(247.f, 0.f) / TexSize;
    else if (R_Level == 2)
        Skill_R_discardRegionMin = float2(258.f, 0.f) / TexSize;
    else if (R_Level == 3)
        Skill_R_discardRegionMin = float2(269.f, 0.f) / TexSize;
    //else if (R_Level == 5)
    //    Skill_R_discardRegionMin = float2(279.f, 0.f) / TexSize;


        // 현재 UV 좌표가 해당 영역에 속하는지 확인
    if (_in.vUV.x >= Skill_Q_discardRegionMin.x && _in.vUV.x <= Skill_Q_discardRegionMax.x &&
        _in.vUV.y >= Skill_Q_discardRegionMin.y && _in.vUV.y <= Skill_Q_discardRegionMax.y)
    {
        discard; // 해당 영역의 픽셀은 렌더링하지 않음
    }
    
            // 현재 UV 좌표가 해당 영역에 속하는지 확인
    if (_in.vUV.x >= Skill_W_discardRegionMin.x && _in.vUV.x <= Skill_W_discardRegionMax.x &&
        _in.vUV.y >= Skill_W_discardRegionMin.y && _in.vUV.y <= Skill_W_discardRegionMax.y)
    {
        discard; // 해당 영역의 픽셀은 렌더링하지 않음
    }

    
            // 현재 UV 좌표가 해당 영역에 속하는지 확인
    if (_in.vUV.x >= Skill_E_discardRegionMin.x && _in.vUV.x <= Skill_E_discardRegionMax.x &&
        _in.vUV.y >= Skill_E_discardRegionMin.y && _in.vUV.y <= Skill_E_discardRegionMax.y)
    {
        discard; // 해당 영역의 픽셀은 렌더링하지 않음
    }

    
            // 현재 UV 좌표가 해당 영역에 속하는지 확인
    if (_in.vUV.x >= Skill_R_discardRegionMin.x && _in.vUV.x <= Skill_R_discardRegionMax.x &&
        _in.vUV.y >= Skill_R_discardRegionMin.y && _in.vUV.y <= Skill_R_discardRegionMax.y)
    {
        discard; // 해당 영역의 픽셀은 렌더링하지 않음
    }

    return vOutColor;
}








// ============================
// AlphaDurationShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less

// g_tex_0              : Output Texture
// g_int_0              : 알파 라티오 사용여부
// g_float_0            : 알파 비율
// ============================

VS_OUT VS_AlphaDurationShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV; // UV 조정은 삭제

    return output;
}

float4 PS_AlphaDurationShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    if (g_int_0 == 1) // 알파 라티오 사용할경우
    {
        vOutColor.a *= g_float_0;
    }
    return vOutColor;
}


#endif
