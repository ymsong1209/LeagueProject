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

    float3x3 RotationMatrix = float3x3(CosTheta, 0.0, SinTheta, 0.0, 1.0, 0.0, -SinTheta, 0.0, CosTheta);
    
	return mul(_InitialDir, RotationMatrix);
}

//IntersectResult IntersectRay(float3* _Vertices, tRay _ray)
IntersectResult IntersectRay(float3 _Vertices0, float3 _Vertices1, float3 _Vertices2, tRay _ray)
{
    IntersectResult result;
    result.vCrossPoint = float3(0.f, 0.f, 0.f);
    result.bResult = false;

    float3 edge[2] = { float3(0, 0, 0), float3(0, 0, 0) };
    edge[0] = _Vertices1.xyz - _Vertices0.xyz;
    edge[1] = _Vertices2.xyz - _Vertices0.xyz;
    
    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _ray.vDir);

    float3 w0 = _ray.vStart - _Vertices0;
    float a = -(dot(normal, w0));
    float t = a / b;

    result.fResult = t;

    float3 p = _ray.vStart + t * _ray.vDir;

    result.vCrossPoint = p;

    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _Vertices0;
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
IntersectResult CalculateBtwRayCube(tRay Ray, row_major matrix WorldMat)
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
    {
    
        for (int j = 0; j < 3; ++j)
        {
        
            arrLocal[i][j] = mul(float4(arrLocal[i][j], 1.f), WorldMat).xyz;
        }
    }
    
    IntersectResult Final1 = (float3(0.f, 0.f, 0.f), 0.f, false);
    IntersectResult Final2 = (float3(0.f, 0.f, 0.f), 0.f, false);
    IntersectResult Temp;

    for (int k = 0 ; k < 6; ++k)
    {
        Temp = IntersectRay(arrLocal[k][0], arrLocal[k][1], arrLocal[k][2], Ray);

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

    return Final1;

    //// ���࿡ ������ �ϳ��� ������ ���,
    //// �����ڸ��� �ִ� ������ ������ ��ǥ�� �������ش�.
    //if (Final1.bResult = false)
    //{
    //    float3 InitialRayPos = Ray.vStart;

    //    InitialRayPos.x = InitialRayPos.x + _SightRadius * Ray.vDir.x;
    //    InitialRayPos.y = InitialRayPos.y + _SightRadius * Ray.vDir.y;
    //    InitialRayPos.z = InitialRayPos.z + _SightRadius * Ray.vDir.z;

    //    Final1.vCrossPoint.xyz = InitialRayPos.xyz;
    //}

    ////_OutputBuffer[_NthObj][_NthRay] = Final1.vCrossPoint;
    //_OutputBuffer[_RayCountPerObj * _NthObj + _NthRay] = Final1.vCrossPoint;
    }


StructuredBuffer<tRayLightInfo> RAYINFO : register(t16); // Ray�� ���� ������ (CenterPos �̿��� ����)
StructuredBuffer<tColliderInfo> COLLIDERINFO : register(t17); // Collider�� ���� ������
StructuredBuffer< tRayOutput> RAYOUTPUT : register(u0); // Unordered Access

#define SourceLightCount        g_int_0
#define SourceLightPerRay       g_int_1
#define ColliderVecCount        g_int_2


[numthreads(16, 64, 1)]
void  CS_FogOfWarShader(int3 _iThreadID : SV_DispatchThreadID)
{
    // �̺κп� ���ؼ� Ȯ���� ����... �����鿡�� ������� �ҵ� 
    // SourceLightCount, SourceLightPerRay ������ ���ǵǾ� �־�� ��. (#define �� ���� �ɵ� ������ۿ� ���ε��ؼ�)
    if (SourceLightCount  <= _iThreadID.x || SourceLightPerRay <= _iThreadID.y)
        return;

    
    tRay MainRay;
    tRay SupportRay;

    // Ray �������� ��ǥ�� Ray�� �� Object�� ��ǥ���� y���� 10���� ����
    MainRay.vStart.x = RAYINFO[(uint)_iThreadID.x].vRayLightCenterPos.x;
    MainRay.vStart.y = 10.f;
    MainRay.vStart.z = RAYINFO[(uint)_iThreadID.x].vRayLightCenterPos.z;


    SupportRay.vStart.x = RAYINFO[(uint) _iThreadID.x].vRayLightCenterPos.x;
    SupportRay.vStart.y = 10.f;
    SupportRay.vStart.z = RAYINFO[(uint) _iThreadID.x].vRayLightCenterPos.z;

    
    float3 BaseDir = { 1.f, 0.f, 0.f };


    float MainRayAngle = (2 * 3.1415926535f / SourceLightPerRay) * _iThreadID.y;
    float SupportRayAngle = (2 * 3.1415926535f / SourceLightPerRay) * _iThreadID.y;

    MainRay.vDir = RotateVec3Y(BaseDir, MainRayAngle);
    SupportRay.vDir = RotateVec3Y(BaseDir, SupportRayAngle);


    IntersectResult FinalMainRayIntersect = (float3(0.f, 0.f, 0.f), 0.f, false);
    IntersectResult FinalSupportRayIntersect = (float3(0.f, 0.f, 0.f), 0.f, false);


    // Int ���� 1 �ϰ�� Box, Int ���� 0 �ϰ�� Sphere
    // #define ���� ��ü ��� Collider�� ����ؾ��ϴ��� ������ۿ� �޾ƿ;���
    for (int i = 0; i < ColliderVecCount; ++i)
    {
        IntersectResult TempMainRayIntersect = (float3(0.f, 0.f, 0.f), 0.f, false);
        IntersectResult TempSupportRayIntersect = (float3(0.f, 0.f, 0.f), 0.f, false);

        // Box�� ���
        if (COLLIDERINFO[i].iColliderType == 1)
        {
            // ����ȭ ���� ��򰡿��� Radius�� ���� ���� �޾ƿ;� �� �� ����
            TempMainRayIntersect = CalculateBtwRayCube(MainRay, COLLIDERINFO[i].mColliderFinalMat);
            TempSupportRayIntersect = CalculateBtwRayCube(SupportRay, COLLIDERINFO[i].mColliderFinalMat);
        }

        else if (COLLIDERINFO[i].iColliderType == 0)
        {

        }


        // ������ �ִ� �Ͱ� �񱳸� �ؾ��� �׷��� �� ������ �ִ� Coillider�� ������ ������ �� ����
        if (FinalMainRayIntersect.bResult == false && TempMainRayIntersect.bResult == true)
        {
            FinalMainRayIntersect = TempMainRayIntersect;
        }

        else if (FinalMainRayIntersect.bResult == true)
        {
            if (TempMainRayIntersect.bResult == true &&  TempMainRayIntersect.fResult <= FinalMainRayIntersect.fResult)
            {
                FinalMainRayIntersect = TempMainRayIntersect;
            }
        }

        if (FinalSupportRayIntersect.bResult == false)
        {
            FinalSupportRayIntersect = TempSupportRayIntersect;
        }

        else if (FinalSupportRayIntersect.bResult == true)
        {
            if (TempSupportRayIntersect.fResult <= FinalSupportRayIntersect.fResult)
            {
                FinalSupportRayIntersect = TempSupportRayIntersect;
            }
        }
    }

    // �������� ã������ �ۼ��ϴ� �ڵ�
    IntersectResult InterpolateIntersect;

    // ������ �����ٴ� ����. �̷��ԵǸ� �������� ���� Ray�� ������ �������� ������ ��ǥ�� ��� �� ���̴�.
    if (FinalMainRayIntersect.bResult == false)
    {
        float3 InitialRayPos = MainRay.vStart;

        // Sight Radius�� Ray�� ��� ������ ������ ����
        InitialRayPos.x = InitialRayPos.x + RAYINFO[_iThreadID.x].fRayRange * MainRay.vDir.x;
        InitialRayPos.y = 10.f;
        InitialRayPos.z = InitialRayPos.z + RAYINFO[_iThreadID.x].fRayRange * MainRay.vDir.z;

        FinalMainRayIntersect.vCrossPoint.xyz = InitialRayPos.xyz;
        FinalMainRayIntersect.fResult = RAYINFO[_iThreadID.x];
        FinalMainRayIntersect.bResult = true;
    }

    if (FinalSupportRayIntersect.bResult == false)
    {
        float3 InitialRayPos = SupportRay.vStart;

        // Sight Radius�� Ray�� ��� ������ ������ ����
        InitialRayPos.x = InitialRayPos.x + RAYINFO[_iThreadID.x].fRayRange * SupportRay.vDir.x;
        InitialRayPos.y = 10.f;
        InitialRayPos.z = InitialRayPos.z + RAYINFO[_iThreadID.x].fRayRange * SupportRay.vDir.z;

        FinalSupportRayIntersect.vCrossPoint.xyz = InitialRayPos.xyz;
        FinalSupportRayIntersect.fResult = RAYINFO[_iThreadID.x];
        FinalSupportRayIntersect.bResult = true;
    }

    InterpolateIntersect.fResult = (FinalMainRayIntersect.fResult + FinalSupportRayIntersect.fResult) / 2.f;

    float3 RayObjCenterPos = MainRay.vStart;
    RayObjCenterPos.x = RayObjCenterPos.x + InterpolateIntersect.fResult * MainRay.vDir.x;
    RayObjCenterPos.y = 10.f;
    RayObjCenterPos.z = RayObjCenterPos.z + InterpolateIntersect.fResult * MainRay.vDir.z;

    InterpolateIntersect.vCrossPoint.xyz = RayObjCenterPos.xyz;

    // Read Write����ü�� ��Ƽ� ���� ��ȯ�������.
    tRayOutput OutputResult;
    OutputResult.IntersectPos.xyz = InterpolateIntersect.vCrossPoint.xyz;
    OutputResult.Radius = InterpolateIntersect.fResult;
    OutputResult.CenterPos = RayObjCenterPos;
    OutputResult.NthRay = (int)((uint)_iThreadID.x % (uint)SourceLightPerRay);

    RAYOUTPUT[_iThreadID.x * SourceLightPerRay + _iThreadID.y] = OutputResult;
}

#endif