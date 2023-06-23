#ifndef _DEBUGBOUNDING
#define _DEBUGBOUNDING


#include "value.fx"
#include "func.fx"
#include "struct.fx"

// ========================
// DebugBounding Shader

// Topology : LineStrip
// RS_TYPE  : CULL_NONE
// DS_TYPE  : NO_TEST_NO_WRITE
// BS_TYPE  : Default
// g_float_0 : Bounding
// g_mat_0 : matViewRot
// ========================


struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float3 vPosition : POSITION;
};

VS_OUT VS_DebugBounding(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = _in.vPos;
    
    return output;
}


// GeometryShader ���
// 1. ���������� ����
// 2. ������ ó�� (ī�޶� �ٶ󺸴�..)
struct GS_OUT
{
    float4 vPosition : SV_Position;
};
[maxvertexcount(140)]
void GS_DebugBounding(point VS_OUT _in[1], inout LineStream<GS_OUT> _outstream)
{
    float3 vCenter = _in[0].vPosition.xyz;
    float fRadius = g_float_0;

   // x�� ����
    for (int i = 0; i <= 40; ++i)
    {
        float angle = i * 0.15707963267949f; // (XM_2PI) / 40
        float x = vCenter.x + fRadius * cos(angle);
        float y = vCenter.y;
        float z = vCenter.z + fRadius * sin(angle);
  
        GS_OUT vertex;
        vertex.vPosition = mul(float4(x, y, z, 1.0f), g_matWVP);
        _outstream.Append(vertex);
    }
    
   // z�� ����
    for (int i = 0; i <= 40; ++i)
    {
        float angle = i * 0.15707963267949f; // (XM_2PI) / 40
        float x = vCenter.x + fRadius * cos(angle);
        float y = vCenter.y + fRadius * sin(angle);
        float z = vCenter.z;
   
        GS_OUT vertex;
        vertex.vPosition = mul(float4(x, y, z, 1.0f), g_matWVP);
        _outstream.Append(vertex);
    }
    
    // y�� ������ ��⿡ �ռ� ���� �ִ� �� ��ġ�� �̵������ش�.
    for (int i = 0; i <= 10; ++i)
    {
        float angle = i * 0.15707963267949f; // (XM_2PI) / 40
        float x = vCenter.x + fRadius * cos(angle);
        float y = vCenter.y + fRadius * sin(angle);
        float z = vCenter.z;
   
        GS_OUT vertex;
        vertex.vPosition = mul(float4(x, y, z, 1.0f), g_matWVP);
        _outstream.Append(vertex);
    }
    
    
    // y�� ����
    for (int i = 0; i <= 40; ++i)
    {
        float angle = i * 0.15707963267949f; // (XM_2PI) / 40
        float x = vCenter.x;
        float y = vCenter.y + fRadius * cos(angle);
        float z = vCenter.z + fRadius * sin(angle);
    
        GS_OUT vertex;
        vertex.vPosition = mul(float4(x, y, z, 1.0f), g_matWVP);
        _outstream.Append(vertex);
    }
    
    float angle = 10 * 0.15707963267949f; // (XM_2PI) / 40
    float x = vCenter.x + fRadius * cos(angle);
    float y = vCenter.y + fRadius * sin(angle);
    float z = vCenter.z;
   
    GS_OUT vertex;
    vertex.vPosition = mul(float4(x, y, z, 1.0f), g_matWVP);
    _outstream.Append(vertex);
}

// Rasterizer

float4 PS_DebugBounding(GS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 1.f, 1.f);
   
    return vOutColor;
}



#endif
