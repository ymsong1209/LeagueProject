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
    result.fResult = 1000000.f;

    float3 edge[2] = { float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f) };
    edge[0] = _Vertices1.xyz - _Vertices0.xyz;
    edge[1] = _Vertices2.xyz - _Vertices0.xyz;
    
    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _ray.vDir);

    float3 w0 = _ray.vStart - _Vertices0.xyz;
    float a = -dot(normal, w0);
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


// 여기서 N은 0부터 시작한다. 0 <= _NthRay < _RayCountPerObj
IntersectResult CalculateBtwRayCube(tRay Ray, row_major matrix WorldMat, inout int _check)
{
    float3 arrLocal[6][3] =
    {
        {float3(-0.5f, 0.5f, -0.5f),  float3(0.5f, 0.5f, -0.5f),  float3(-0.5f, 0.5f, 0.5f)},	 // 윗면
        {float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, -0.5f, 0.5f)}, // 밑면
        {float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, 0.5f, -0.5f)}, // 앞면
        {float3(-0.5f, -0.5f, 0.5f),  float3(0.5f, -0.5f, 0.5f),  float3(-0.5f, 0.5f, 0.5f)},  // 뒷면
        {float3(-0.5f, 0.5f, -0.5f),  float3(-0.5f, -0.5f, -0.5f),float3(-0.5f, 0.5f, 0.5f)},  // 왼쪽면
        {float3(0.5f, 0.5f, -0.5f),   float3(0.5f, -0.5f, -0.5f), float3(0.5f, 0.5f, 0.5f)},   // 오른쪽면
    };

    for (int i = 0; i < 6; ++i)
    {
    
        for (int j = 0; j < 3; ++j)
        {
        
            arrLocal[i][j] = mul(float4(arrLocal[i][j], 1.f), WorldMat).xyz;
        }
    }
    

    IntersectResult Final1;
    Final1.vCrossPoint = float3(0.f, 0.f, 0.f);
    Final1.fResult = 0.f;
    Final1.bResult = false;

    IntersectResult Final2;
    Final2.vCrossPoint = float3(0.f, 0.f, 0.f);
    Final2.fResult = 0.f;
    Final2.bResult = false;
    
    
    IntersectResult Temp;

    for (int k = 0 ; k < 6; ++k)
    {
        Temp = IntersectRay(arrLocal[k][0], arrLocal[k][1], arrLocal[k][2], Ray);

        if (Temp.bResult == true)
        {
        // 첫번째로 충돌된 경우
            if (Final1.bResult == false)
                Final1 = Temp;
            else if (Final1.bResult == true && Final2.bResult == false)
            {
            // Ray와 Cube의 두면에서 교점이 생긴 상황이며, 이제 멀리있는 것이 Final1이 되도록
            // 순서를 바꿔야 한다.
            // 왜냐하면, Final1에 있는 정보들이 중요한데, 시야 판단의 기준이 되기 때문이다.
                if (Final1.fResult < Temp.fResult)
                {
                    IntersectResult SwapTemp;
                    SwapTemp.bResult = Final1.bResult;
                    SwapTemp.fResult = Final1.fResult;
                    SwapTemp.vCrossPoint = Final1.vCrossPoint;
                    
                    Final1.bResult = Temp.bResult;
                    Final1.fResult = Temp.fResult;
                    Final1.vCrossPoint = Temp.vCrossPoint;
                    
                    Final2.bResult = SwapTemp.bResult;
                    Final2.fResult = SwapTemp.fResult;
                    Final2.vCrossPoint = SwapTemp.vCrossPoint;
             
                }
                
                else
                {
                    Final2 = Temp;
                }
            }
            else if (Final1.bResult == true && Final2.bResult == true)
            {
                _check += 1000;
            }
        }
        else
        {
            _check += 100;
        }
    }

    return Final1;

    //// 만약에 교점이 하나도 없었던 경우,
    //// 가장자리에 있는 점으로 교점의 좌표를 세팅해준다.
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


StructuredBuffer<tRayLightInfo> RAYINFO : register(t16); // Ray에 대한 정보들 (CenterPos 이용할 것임)
StructuredBuffer<tColliderInfo> COLLIDERINFO : register(t17); // Collider에 대한 정보들
RWStructuredBuffer<tRayOutput> RAYOUTPUT : register(u0); // Unordered Access

#define SourceLightCount        g_int_0
#define RayCountperObject       g_int_1
#define ColliderVecCount        g_int_2


[numthreads(16, 64, 1)]
void  CS_FogOfWarShader(int3 _iThreadID : SV_DispatchThreadID)
{
    // 이부분에 대해서 확신이 없음... 팀원들에게 물어봐야 할듯 
    // SourceLightCount, RayCountperObject 변수가 정의되어 있어야 함. (#define 문 쓰면 될듯 상수버퍼에 바인딩해서)
    if (SourceLightCount  <= _iThreadID.x || RayCountperObject <= _iThreadID.y)
        return;

    
    tRay MainRay;
    tRay SupportRay;

    // Ray 시작점의 좌표는 Ray를 쏠 Object의 좌표에서 y값만 10으로 고정
    MainRay.vStart.x = RAYINFO[(uint)_iThreadID.x].vRayLightCenterPos.x;
    MainRay.vStart.y = 10.f;
    MainRay.vStart.z = RAYINFO[(uint)_iThreadID.x].vRayLightCenterPos.z;


    SupportRay.vStart.x = RAYINFO[(uint) _iThreadID.x].vRayLightCenterPos.x;
    SupportRay.vStart.y = 10.f;
    SupportRay.vStart.z = RAYINFO[(uint) _iThreadID.x].vRayLightCenterPos.z;

    
    float3 BaseDir = { 1.f, 0.f, 0.f };


    float MainRayAngle = (2 * 3.1415926535f / RayCountperObject) * _iThreadID.y;
    float SupportRayAngle = (2 * 3.1415926535f / RayCountperObject) * (_iThreadID.y + 1);

    MainRay.vDir = RotateVec3Y(BaseDir, MainRayAngle);
    SupportRay.vDir = RotateVec3Y(BaseDir, SupportRayAngle);
    
    MainRay.vDir = normalize(MainRay.vDir);
    SupportRay.vDir = normalize(SupportRay.vDir);


    IntersectResult FinalMainRayIntersect;
    FinalMainRayIntersect.vCrossPoint = float3(0.f, 0.f, 0.f);
    FinalMainRayIntersect.fResult = 0.f;
    FinalMainRayIntersect.bResult = false;
    
    IntersectResult FinalSupportRayIntersect;
    FinalSupportRayIntersect.vCrossPoint = float3(0.f, 0.f, 0.f);
    FinalSupportRayIntersect.fResult = 0.f;
    FinalSupportRayIntersect.bResult = false;

    int check = 0;

    // Int 값이 1 일경우 Box, Int 값이 0 일경우 Sphere
    // #define 으로 전체 몇개의 Collider를 계산해야하는지 상수버퍼에 받아와야함
    for (int i = 0; i < ColliderVecCount; ++i)
    {
        IntersectResult TempMainRayIntersect;
        TempMainRayIntersect.vCrossPoint = float3(0.f, 0.f, 0.f);
        TempMainRayIntersect.fResult = 0.f;
        TempMainRayIntersect.bResult = false;
        
        IntersectResult TempSupportRayIntersect;
        TempSupportRayIntersect.vCrossPoint = float3(0.f, 0.f, 0.f);
        TempSupportRayIntersect.fResult = 0.f;
        TempSupportRayIntersect.bResult = false;

        // Box인 경우
        if (COLLIDERINFO[i].iColliderType == 1)
        {
            // 구조화 버퍼 어딘가에서 Radius에 대한 정보 받아와야 할 것 같음
            TempMainRayIntersect = CalculateBtwRayCube(MainRay, COLLIDERINFO[i].mColliderFinalMat, check);
            TempSupportRayIntersect = CalculateBtwRayCube(SupportRay, COLLIDERINFO[i].mColliderFinalMat, check);
        }

        else if (COLLIDERINFO[i].iColliderType == 0)
        {

        }


        // 기존에 있던 것과 비교를 해야함 그래서 더 가까이 있는 Coillider의 정보로 갱신을 할 것임
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

        if (FinalSupportRayIntersect.bResult == false && TempSupportRayIntersect.bResult == true)
        {
            FinalSupportRayIntersect = TempSupportRayIntersect;
        }

        else if (FinalSupportRayIntersect.bResult == true)
        {
            if (TempSupportRayIntersect.bResult == true && TempSupportRayIntersect.fResult <= FinalSupportRayIntersect.fResult)
            {
                FinalSupportRayIntersect = TempSupportRayIntersect;
            }
        }
    }

    // 보간점을 찾을려고 작성하는 코드
    IntersectResult InterpolateIntersect;
   

    // 교점이 없었다는 뜻임. 이렇게되면 반지름의 경계와 Ray가 만나는 지점으로 교점의 좌표를 잡아 줄 것이다.
    if (FinalMainRayIntersect.bResult == false)
    {
        float3 InitialRayPos = MainRay.vStart;

        // Sight Radius는 Ray를 쏘는 광원의 반지름 값임
        InitialRayPos.x = InitialRayPos.x + RAYINFO[_iThreadID.x].fRayRange * MainRay.vDir.x;
        InitialRayPos.y = 100.f;
        InitialRayPos.z = InitialRayPos.z + RAYINFO[_iThreadID.x].fRayRange * MainRay.vDir.z;

        FinalMainRayIntersect.vCrossPoint.xyz = InitialRayPos.xyz;
        FinalMainRayIntersect.fResult = RAYINFO[_iThreadID.x].fRayRange;
        FinalMainRayIntersect.bResult = true;
    }

    if (FinalSupportRayIntersect.bResult == false)
    {
        float3 InitialRayPos = SupportRay.vStart;

        // Sight Radius는 Ray를 쏘는 광원의 반지름 값임
        InitialRayPos.x = InitialRayPos.x + RAYINFO[_iThreadID.x].fRayRange * SupportRay.vDir.x;
        InitialRayPos.y = 100.f;
        InitialRayPos.z = InitialRayPos.z + RAYINFO[_iThreadID.x].fRayRange * SupportRay.vDir.z;

        FinalSupportRayIntersect.vCrossPoint.xyz = InitialRayPos.xyz;
        FinalSupportRayIntersect.fResult = RAYINFO[_iThreadID.x].fRayRange;
        FinalSupportRayIntersect.bResult = true;
    }

    InterpolateIntersect.fResult = (FinalMainRayIntersect.fResult + FinalSupportRayIntersect.fResult) / 2.f;

    float3 CrossPointFinalPos = MainRay.vStart;
    CrossPointFinalPos.x = CrossPointFinalPos.x + InterpolateIntersect.fResult * MainRay.vDir.x;
    CrossPointFinalPos.y = (FinalMainRayIntersect.vCrossPoint.y + FinalSupportRayIntersect.vCrossPoint.y) / 2.f;
    CrossPointFinalPos.z = CrossPointFinalPos.z + InterpolateIntersect.fResult * MainRay.vDir.z;
    

    InterpolateIntersect.vCrossPoint.xyz = CrossPointFinalPos.xyz;

    // Read Write구조체에 담아서 값을 반환해줘야함.
    tRayOutput OutputResult;
    OutputResult.IntersectPos.xyz = InterpolateIntersect.vCrossPoint.xyz;
    OutputResult.Radius = InterpolateIntersect.fResult;
    OutputResult.CenterPos = MainRay.vStart;
    OutputResult.NthRay = _iThreadID.y;
    OutputResult.check = check;
    OutputResult.pad[0] = 0.f;
    OutputResult.pad[1] = 0.f;
    OutputResult.pad[2] = 0.f;
    
 

    RAYOUTPUT[_iThreadID.x * RayCountperObject + _iThreadID.y] = OutputResult;
}

#endif