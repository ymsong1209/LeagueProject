#ifndef _FOG_OF_WAR
#define _FOG_OF_WAR

#include "value.fx"
#include "struct.fx"
#include "func.fx"

struct tRay
{
	float3 vStart;
	float3 vDir;
};

struct IntersectResult
{
	float3 vCrossPoint;
	float  fResult;
	bool   bResult;
};

float3 RotateVec3Y(float3 _InitialDir, float _Angle)
{
	float CosTheta = cos(_Angle);
	float SinTheta = sin(_Angle);

	float3x3 RotationMatrix = float3x3 {
			CosTheta, 0.0, SinTheta,
			0.0, 1.0, 0.0,
			-SinTheta, 0.0, CosTheta
	};

	return mul(_InitialDir, RotationMatrix);
}

IntersectResult IntersectRay(float3* _Vertices, tRay _ray)
{
    IntersectResult result;
    result.vCrossPoint = float3(0.f, 0.f, 0.f);
    result.bResult = false;

    float3 edge[2] = { float3(), float3() };
    edge[0] = _Vertices[1].xyz - _Vertices[0].xyz;
    edge[1] = _Vertices[2].xyz - _Vertices[0].xyz;

    float3 normal = cross(edge[0], edge[1]).normalize();
    float b = dot(normal, _ray.vDir);

    float3 w0 = _ray.vStart - _Vertices[0];
    float a = -(dot(normal, w0));
    float t = a / b;

    result.fResult = t;

    float3 p = _ray.vStart + t * _ray.vDir;

    result.vCrossPoint = p;

    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _Vertices[0];
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);

    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        result.vCrossPoint = float3(0.f, 0.f, 0.f);
        result.fResult = 0.0f;
        result.bResult = false;
        return result;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || v > 1.0f)
    {
        result.vCrossPoint = float3(0.f, 0.f, 0.f);
        result.fResult = 0.0f;
        result.bResult = false;
        return result;
    }

    result.bResult = true;
    return result;
}


// ���⼭ N�� 0���� �����Ѵ�. 0 <= _NthRay < _RayCountPerObj
void CalculateBtwRayCube(tRay& Ray, row_major matrix WorldMat, int _NthRay, int _NthObj, 
    int _RayCountPerObj, float _SightRadius,  CStructuredBuffer* _OutputBuffer)
{
    float3 arrLocal[6][3] =
    {
        {float3(-0.5f, 0.5f, -0.5f),  float3(0.5f, 0.5f, -0.5f),  float3(-0.5f, 0.5f, 0.5f)},	 // ����
        {float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, -0.5f, 0.5f)}, // �ظ�
        {float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, 0.5f, -0.5f)}, // �ո�
        {float3(-0.5f, -0.5f, 0.5f),  float3(0.5f, -0.5f, 0.5f),  float3(-0.5f, 0.5f, 0.5f)},  // �޸�
        {float3(-0.5f, 0.5f, -0.5f),  float3(-0.5f, -0.5f, -0.5f),float3(-0.5f, 0.5f, 0.5f)},  // ���ʸ�
        {float3(0.5f, 0.5f, -0.5f),   float3(0.5f, -0.5f, -0.5f), float3(0.5f, 0.5f, 0.5f)},   // �����ʸ�
    };

    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 3; ++j)
            arrLocal[i][j] = mul(float4(arrLocal[i][j], 1.f), WorldMat).xyz;

    IntersectResult Final1 = IntersectResult{ float3(0.f, 0.f, 0.f), 0.f, false };
    IntersectResult Final2 = IntersectResult{ float3(0.f, 0.f, 0.f), 0.f, false };
    IntersectResult Temp;

    for (int i = 0; i < 6; ++i)
    {
        Temp = IntersectRay(arrLocal[i], Ray);

        if (Temp.bResult == true)
        {
            // ù��°�� �浹�� ���
            if (Final1.bResult == false)
                Final1 = Temp;
            else if (Final1.bResult == true && Final2.bResult == false)
            {
                // Ray�� Cube�� �θ鿡�� ������ ���� ��Ȳ�̸�, ���� �ָ��ִ� ���� Final1�� �ǵ���
                // ������ �ٲ�� �Ѵ�.
                // �ֳ��ϸ�, Final1�� �ִ� �������� �߿��ѵ�, �þ� �Ǵ��� ������ �Ǳ� �����̴�.
                if (Final1.fResult < Temp.fResult)
                {
                    IntersectResult SwapTemp;
                    SwapTemp = Final1;
                    Final1 = Temp;
                    Final2 = SwapTemp;
                }

                else
                {
                    Final2 = Temp;
                }
            }
        }
    }

    // ���࿡ ������ �ϳ��� ������ ���,
    // �����ڸ��� �ִ� ������ ������ ��ǥ�� �������ش�.
    if (Final1.bResult = false)
    {
        float3 InitialRayPos = Ray.vStart;

        InitialRayPos.x = InitialRayPos.x + _SightRadius * Ray.vDir.x;
        InitialRayPos.y = InitialRayPos.y + _SightRadius * Ray.vDir.y;
        InitialRayPos.z = InitialRayPos.z + _SightRadius * Ray.vDir.z;

        Final1.vCrossPoint.xyz = InitialRayPos.xyz;
    }

    //_OutputBuffer[_NthObj][_NthRay] = Final1.vCrossPoint;
    _OutputBuffer[_RayCountPerObj * _NthObj + _NthRay] = Final1.vCrossPoint;
}


// #define ���� ���� �ȵ� ������ۿ� �ִ� ���� ������ �ٲ���� ��
// ex) #define LightCount = g_int_1


// �����ڷ�. FogForwardShader���� ���� ����ƴ�
//RWStructuredBuffer<tWeight_4> WEIGHT_MAP : register(u0); // unordered acess
//StructuredBuffer<tRaycastOut> LOCATION : register(t16); // �귯�� ��ġ(�»�� ����)


StructuredBuffer<tRayLightInfo> RAYINFO : register(t16); // Ray�� ���� ������ (CenterPos �̿��� ����)
StructuredBuffer< tColliderInfo> COLLIDERINFO : register(t17); // Collider�� ���� ������


[numthreads(16, 64, 1)]
void  CS_FogOfWarShader(int3 _iThreadID : SV_DispatchThreadID)
{

    // �����ڷ�. � �����尡 ���� ���� ���������� Ȯ���ϱ����ؼ� weightmap.fx���� ��������
    /*  if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].success)
    {
        return;
    }*/

    // �̺κп� ���ؼ� Ȯ���� ����... �����鿡�� ������� �ҵ� 
    // SourceLightCount, SourceLightPerRay ������ ���ǵǾ� �־�� ��. (#define �� ���� �ɵ� ������ۿ� ���ε��ؼ�)
    if (SourceLightCount * SourceLightPerRay <= _iThreadID.x || SourceLightPerRay <= _iThreadID.y)
        return;

    
    tRay MainRay;
    tRay SupportRay;

    // Ray �������� ��ǥ�� Ray�� �� Object�� ��ǥ���� y���� 10���� ����
    MainRay.vStart.x = RAYINFO[(UINT)_iThreadID.x / (UINT)SourceLightPerRay].vRayLightCenterPos.x;
    MainRay.vStart.y = 10.f;
    MainRay.vStart.z = RAYINFO[(UINT)_iThreadID.x / (UINT)SourceLightPerRay].vRayLightCenterPos.z;


    SupportRay.vStart.x = RAYINFO[(UINT)_iThreadID.x / (UINT)SourceLightPerRay].vRayLightCenterPos.x;
    SupportRay.vStart.y = 10.f;
    SupportRay.vStart.z = RAYINFO[(UINT)_iThreadID.x / (UINT)SourceLightPerRay].vRayLightCenterPos.z;

    
    float3 BaseDir = { 1.f, 0.f, 0.f };


    float MainRayAngle = (2 * 3.1415926535f / SourceLightPerRay) * ((UINT)_iThreadID.x % (UINT)SourceLightPerRay);
    float SupportRayAngle = (2 * 3.1415926535f / SourceLightPerRay) * ((UINT)(_iThreadID.x + 1) % (UINT)SourceLightPerRay);

    MainRay.vDir = RotateVec3Y(BaseDir, MainRayAngle);
    SupportRay.vDir = RotateVec3Y(BaseDir, SupportRayAngle);

    // Int ���� 1 �ϰ�� Box, Int ���� 0 �ϰ�� Sphere
    // #define ���� ��ü ��� Collider�� ����ؾ��ϴ��� ������ۿ� �޾ƿ;���
    for (int i = 0; i < ColliderVecCount; ++i)
    {
        float3 MainRayIntersectResult[][];
        float3 SupportRayIntersectResult[][];

        // Box�� ���
        if (COLLIDERINFO[i].iColliderType == 1)
        {
            // ����ȭ ���� ��򰡿��� Radius�� ���� ���� �޾ƿ;� �� �� ����
            CalculateBtwRayCube(MainRay, COLLIDERINFO[i].mColliderFinalMat, (int)((UINT)_iThreadID.x % (UINT)SourceLightPerRay),
                (int)((UINT)_iThreadID.x / (UINT)SourceLightPerRay), SourceLightPerRay, SightRadius, MainRayIntersectResult);

            CalculateBtwRayCube(SupportRay, COLLIDERINFO[i].mColliderFinalMat, (int)((UINT)_iThreadID.x % (UINT)SourceLightPerRay),
                (int)((UINT)_iThreadID.x / (UINT)SourceLightPerRay), SourceLightPerRay, SightRadius, SupportRayIntersectResult);
        }

        else if (COLLIDERINFO[i].iColliderType == 0)
        {

        }

        // Return�� OutputBuffer�� MainRayIntersectResult�� SupportRayIntersectResult�� �߰����� ����ؼ� 
        // ��ǥ�� �����ؾ��Ѵ�

    }
}