#define PI 3.1415926535

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

float3 RotateVec3Y(float3 _InitialDirection, float  _rotationAtngle)
{
	float CosAngle = cos(_rotationAngle);
	float SinAngle = sin(_rotationAngle);

	float3 RotatedDir;
	
	RotatedDir.x = _InitialDirection.x * CosAngle + _InitialDirection.z * SinAngle;
	RotatedDir.y = _InitialDirection.y;
	RotatedDir.z = _InitialDirection.z;

	return RotatedDir;
}

IntersectResult IntersectsLay(float3* _Vertices, tRay _ray)
{
	IntersectResult result;
	result.vCrossPoint =  float3(0.f, 0.f, 0.f);
	result.bResult = false;

	float3 edge[2] = { float3(), float3() };
	edge[0] = _vertices[1].xyz - _vertices[0].xyz;
	edge[1] = _vertices[2].xyz - _vertices[0].xyz;

	Vec3 normal = (edge[0].Cross(edge[1])).Normalize();
	float b = normal.Dot(_ray.vDir);


	Vec3 w0 = _ray.vStart - _vertices[0];
	float a = -(normal.Dot(w0));
	float t = a / b;

	result.fResult = t;

	Vec3 p = _ray.vStart + t * _ray.vDir;

	result.vCrossPoint = p;

	float uu, uv, vv, wu, wv, inverseD;
	uu = edge[0].Dot(edge[0]);
	uv = edge[0].Dot(edge[1]);
	vv = edge[1].Dot(edge[1]);

	Vec3 w = p - _vertices[0];
	wu = w.Dot(edge[0]);
	wv = w.Dot(edge[1]);

	inverseD = uv * uv - uu * vv;
	inverseD = 1.0f / inverseD;

	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.0f || u > 1.0f)
	{
		result.vCrossPoint = Vec3();
		result.fResult = 0.0f;
		result.bResult = false;
		return result;
	}

	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.0f || v > 1.0f)
	{
		result.vCrossPoint = Vec3();
		result.fResult = 0.0f;
		result.bResult = false;
		return result;
	}

	result.bResult = true;
	return result;
}


void IsCollidingBtwRayCube(tRay& Ray, row_major matrix WorldMat, int _OutputIndex, CStructuredBuffer* _OutPutBuffer)
{
	float3 arrLocal[6][3] =
	{
		{float3(-0.5f, 0.5f, -0.5f),  float3(0.5f, 0.5f, -0.5f),  float3(-0.5f, 0.5f, 0.5f)},	 // 윗면
		{float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, -0.5f, 0.5f)}, // 밑면
		{float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, 0.5f, -0.5f)}, // 앞면
		{float3(-0.5f, -0.5f, 0.5f),  float3(0.5f, -0.5f, 0.5f),  float3(-0.5f, 0.5f, 0.5f)},  // 뒷면
		{float3(-0.5f, 0.5f, -0.5f),  float3(-0.5f, -0.5f, -0.5f),float3(-0.5f, 0.5f, 0.5f)},  // 왼쪽면
		{float3(0.5f, 0.5f, -0.5f),   float3(0.5f, -0.5f, -0.5f), float3(0.5f, 0.5f, 0.5f)},   // 오른쪽면
	}

	for (int i = 0; i < 6; ++i)
		for (int j = 0; j < 3; ++ + j)
			arrLocal[i][j] = mul(float4(arrLocal[i][j], 1.f), WorldMat).xyz;

	IntersectResult Final1 = IntersectResult { float3(0.f, 0.f, 0.f), 0.f, false };
	IntersectResult Final2 = IntersectResult { float3(0.f, 0.f, 0.f), 0.f, false };
	IntersectResult Temp;

	for (int i = 0; i < 6; ++i)
	{
		Temp = IntersecsLay(arrLocal[i], m_ray);

		if (Temp.bResult == true)
		{
			// 첫번째로 충돌된경우
			if (Final1.bResult == false)
			{
				Final1 = Temp;
			}

			else if (Final1.bResult == true && Final2.bResult == false)
			{
				// 2개의 교점이 들어온 상황이며, 이제 가까운것이 먼저 오도록 순서를 바꿔야 함
				if (Final1.fResult <= Temp.fResult)
				{
					Final2 = Temp;
				}

				else
				{
					IntersectResult SwapTemp;
					SwapTemp = Final1;
					Final1 = Temp;
					Final2 = SwapTemp;
				}
			}
		}
	}


	_OutputBuffer[_OutputIndex].FirstCrossPoint = Final1;
	_OutputBuffer[_OutputIndex].SecondCrossPoint = Final2;
}


[numthreads(256, 1, 1)]
void CS_FogOfShader(int3 _iThreadIdx : SV_DispatchThreadID)
{
	if (RayCount <= _iThreadIdx.x)		// RayCount 라는 변수를 상수버퍼에서 지정해 줘야 함.
		return;							// IThreadIdx.x 로 몇번째 레이인지 알 수 있음 


	tRay FirstRay;
	tRay SecondRay;

	// Ray 시작점의 좌표는 Ray를 쏠 Object의 좌표에서 y값만 10으로 고정
	FirstRay.vStart.x = ObjectPos.x;	// ObjectPos라는 변수를 상수버퍼에서 지정해줘야함
	FirstRay.vStart.y = 10.f;
	FirstRay.vStart.z = ObjectPos.z;

	SecondRay.vStart.x = ObjectPos.x;
	SecondRay.vStart.y = 10.f;
	SecondRay.vStart.z = ObjectPos.z;


	float3 BaseDir = { 1.f, 0.f, 0.f };

	float FirstRayAngle = (PI / RayCount) * _iThreadIdx.x;
	float SecondRayAngle = (PI / RayCoutn) * (_iThreadIdx.x + 1);

	FirstRay.vDir = RotateVec3Y(BaseDir, FirstRayAngle);
	SecondRay.vDir = RotateVec3Y(BaseDir, SecondRayAngle);


	// Collider Type 값을 Int형식으로 반환받아 올 수 있어야 함
	// Int 0 : Box ,  Int 1 : Sphere
	for (int i = 0;  i <  RayCount; ++i)
	{
		// m_vEssentialInfoBuffer에서는 무슨 타입인지 확인을 할 수 있어야함.
		if (m_vEssentialInfoBuffer[_iThreadIdx].iColliderType == 0)    // .iColliderType 이 0이라면 Box
		{

		}

		else if (m_vEssentialInfoBuffer[_iThreadIdx].iColliderType == 1) // .iColliderType 이 1이라면 Sphere
		{

		}
	}






	


}