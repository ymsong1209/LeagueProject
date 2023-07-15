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
		{float3(-0.5f, 0.5f, -0.5f),  float3(0.5f, 0.5f, -0.5f),  float3(-0.5f, 0.5f, 0.5f)},	 // ����
		{float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, -0.5f, 0.5f)}, // �ظ�
		{float3(-0.5f, -0.5f, -0.5f), float3(0.5f, -0.5f, -0.5f), float3(-0.5f, 0.5f, -0.5f)}, // �ո�
		{float3(-0.5f, -0.5f, 0.5f),  float3(0.5f, -0.5f, 0.5f),  float3(-0.5f, 0.5f, 0.5f)},  // �޸�
		{float3(-0.5f, 0.5f, -0.5f),  float3(-0.5f, -0.5f, -0.5f),float3(-0.5f, 0.5f, 0.5f)},  // ���ʸ�
		{float3(0.5f, 0.5f, -0.5f),   float3(0.5f, -0.5f, -0.5f), float3(0.5f, 0.5f, 0.5f)},   // �����ʸ�
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
			// ù��°�� �浹�Ȱ��
			if (Final1.bResult == false)
			{
				Final1 = Temp;
			}

			else if (Final1.bResult == true && Final2.bResult == false)
			{
				// 2���� ������ ���� ��Ȳ�̸�, ���� �������� ���� ������ ������ �ٲ�� ��
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
	if (RayCount <= _iThreadIdx.x)		// RayCount ��� ������ ������ۿ��� ������ ��� ��.
		return;							// IThreadIdx.x �� ���° �������� �� �� ���� 


	tRay FirstRay;
	tRay SecondRay;

	// Ray �������� ��ǥ�� Ray�� �� Object�� ��ǥ���� y���� 10���� ����
	FirstRay.vStart.x = ObjectPos.x;	// ObjectPos��� ������ ������ۿ��� �����������
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


	// Collider Type ���� Int�������� ��ȯ�޾� �� �� �־�� ��
	// Int 0 : Box ,  Int 1 : Sphere
	for (int i = 0;  i <  RayCount; ++i)
	{
		// m_vEssentialInfoBuffer������ ���� Ÿ������ Ȯ���� �� �� �־����.
		if (m_vEssentialInfoBuffer[_iThreadIdx].iColliderType == 0)    // .iColliderType �� 0�̶�� Box
		{

		}

		else if (m_vEssentialInfoBuffer[_iThreadIdx].iColliderType == 1) // .iColliderType �� 1�̶�� Sphere
		{

		}
	}






	


}