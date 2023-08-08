#include "pch.h"
#include "CResMgr.h"


void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT | FMOD_INIT_3D_RIGHTHANDED, nullptr);
}


void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;


	Ptr<CMesh> pMesh = nullptr;

	// ==============
	// PointMesh 생성
	// ==============
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	UINT idx = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddRes(L"PointMesh", pMesh);


	// =============
	// RectMesh 생성
	// =============
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh", pMesh);

	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();


	// ==============
	// Trapezoid Mesh
	// ==============

	v.vPos = Vec3(-0.25f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	vecVtx.push_back(v);

	v.vPos = Vec3(0.25f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"TrapezoidMesh", pMesh);

	vecVtx.clear();
	vecIdx.clear();





	// ===========
	// Circle Mesh
	// ===========
	//      3 
	//   4     2
	//  5 --0-- 1	
	// 반지름
	float fRadius = 0.5f;

	// 각도
	UINT Slice = 40;
	float fTheta = XM_2PI / (float)Slice;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vBinormal = Vec3(0.f, -1.f, 0.f);

	vecVtx.push_back(v);

	// 정점 위치 지정
	for (UINT i = 0; i < Slice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		vecVtx.push_back(v);
	}

	// 인덱스 설정
	for (UINT i = 0; i < Slice - 1; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 마지막 삼각형
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(Slice);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CircleMesh", pMesh);

	vecIdx.clear();
	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CircleMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();


	// =========
	// Cube Mesh
	// =========

	Vtx arrCube[24] = {};
	// 윗면
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// 뒷 면
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// 앞 면
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CubeMesh", pMesh);
	vecIdx.clear();

	///
	//윗면
	//0--1
	//|  |
	//3--2
	//아랫면
	//7---6
	//|   |
	//10--15

	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(10);
	vecIdx.push_back(15);
	vecIdx.push_back(6);
	vecIdx.push_back(10);
	vecIdx.push_back(7);
	vecIdx.push_back(6);
	vecIdx.push_back(1);
	vecIdx.push_back(0);
	vecIdx.push_back(7);
	vecIdx.push_back(15);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh(true);
	pMesh->Create(arrCube, 24, vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CubeMesh_Debug", pMesh);
	vecIdx.clear();


	// ===========
	// Sphere Mesh
	// ===========
	fRadius = 0.5f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	UINT iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVtx.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVtx.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVtx.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"SphereMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	/////////////////
	//Cylinder Mesh//
	/////////////////

	float stackHeight = 1.f / iStackCount;
	float radiusStep = 0.f;
	int ringCount = iStackCount + 1;

	for (int i = 0; i < ringCount; i++) {
		float y = -0.5f * 1.f + i * stackHeight;
		float r = 0.5f + i * radiusStep;
		float dTheta = 2.0f * XM_PI / iSliceCount;
		for (int j = 0; j <= iSliceCount; j++) {
			float c = cos(j * dTheta);
			float s = sin(j * dTheta);

			v.vPos = Vec3(r * c, y, r * s);
			v.vUV = Vec2(static_cast<float>(j) / iSliceCount, 1.0f - static_cast<float>(i) / iStackCount);

			v.vNormal = Vec3(c, 0, s);
			v.vNormal = v.vNormal.Normalize();
			v.vTangent = Vec3(c, 0, -s);
			v.vTangent = v.vTangent.Normalize();
			v.vBinormal.Cross(v.vNormal, v.vTangent);

			vecVtx.push_back(v);
		}
	}

	int ringVertexCount = iSliceCount + 1;
	for (int i = 0; i < iStackCount; i++) {
		for (int j = 0; j < iSliceCount; j++) {
			vecIdx.push_back(i * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j + 1);

			vecIdx.push_back(i * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j + 1);
			vecIdx.push_back(i * ringVertexCount + j + 1);
		}
	}
	//topcap
	int baseIndex = static_cast<int>(vecVtx.size());

	float y = 0.5f;
	float dTheta = 2.0f * XM_PI / iSliceCount;

	for (int i = 0; i <= iSliceCount; i++) {
		float x = 0.5 * cos(i * dTheta);
		float z = 0.5 * sin(i * dTheta);

		v.vPos = Vec3(x, y, z);
		v.vNormal = Vec3(0, 1, 0);
		v.vTangent = Vec3(1, 0, 0);
		v.vBinormal = Vec3(0, 0, -1);
		v.vUV = Vec2(x / 1.f + 0.5f, z / 1.f + 0.5f);
		vecVtx.push_back(v);

	}

	v.vPos = Vec3(0, y, 0);
	v.vNormal = Vec3(0, 1, 0);
	v.vTangent = Vec3(1, 0, 0);
	v.vBinormal = Vec3(0, 0, -1);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	int centerIndex = static_cast<int>(vecVtx.size()) - 1;

	for (int i = 0; i < iSliceCount; i++) {
		vecIdx.push_back(centerIndex);
		vecIdx.push_back(baseIndex + i + 1);
		vecIdx.push_back(baseIndex + i);
	}

	//bottomcap
	baseIndex = static_cast<int>(vecVtx.size());

	y = -0.5f;
	dTheta = 2.0f * XM_PI / iSliceCount;

	for (int i = 0; i <= iSliceCount; i++) {

		float x = 0.5 * cos(i * dTheta);
		float z = 0.5 * sin(i * dTheta);

		v.vPos = Vec3(x, y, z);
		v.vNormal = Vec3(0, -1, 0);
		v.vTangent = Vec3(1, 0, 0);
		v.vBinormal = Vec3(0, 0, 1);
		v.vUV = Vec2(x / 1.f + 0.5f, z / 1.f + 0.5f);
		vecVtx.push_back(v);

	}

	v.vPos = Vec3(0, y, 0);
	v.vNormal = Vec3(0, -1, 0);
	v.vTangent = Vec3(1, 0, 0);
	v.vBinormal = Vec3(0, 0, 1);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);


	centerIndex = static_cast<int>(vecVtx.size()) - 1;

	for (int i = 0; i < iSliceCount; i++) {
		vecIdx.push_back(centerIndex);
		vecIdx.push_back(baseIndex + i);
		vecIdx.push_back(baseIndex + i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CylinderMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	/////////////////
	//Cone Mesh//////
	/////////////////

	stackHeight = 1.f / iStackCount;
	radiusStep = -0.5f / iStackCount;
	ringCount = iStackCount + 1;

	for (int i = 0; i < ringCount; i++) {
		float y = -0.5f * 1.f + i * stackHeight;
		float r = 0.5f + i * radiusStep;
		float dTheta = 2.0f * XM_PI / iSliceCount;
		for (int j = 0; j <= iSliceCount; j++) {
			float c = cos(j * dTheta);
			float s = sin(j * dTheta);

			v.vPos = Vec3(r * c, y, r * s);
			v.vUV = Vec2(static_cast<float>(j) / iSliceCount, 1.0f - static_cast<float>(i) / iStackCount);

			v.vNormal = Vec3(c, 0, s);
			v.vNormal = v.vNormal.Normalize();
			v.vTangent = Vec3(c, 0, -s);
			v.vTangent = v.vTangent.Normalize();
			v.vBinormal.Cross(v.vNormal, v.vTangent);

			vecVtx.push_back(v);
		}
	}

	ringVertexCount = iSliceCount + 1;
	for (int i = 0; i < iStackCount; i++) {
		for (int j = 0; j < iSliceCount; j++) {
			vecIdx.push_back(i * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j + 1);

			vecIdx.push_back(i * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j + 1);
			vecIdx.push_back(i * ringVertexCount + j + 1);
		}
	}
	//topcap
	baseIndex = static_cast<int>(vecVtx.size());

	y = 0.5f;
	dTheta = 2.0f * XM_PI / iSliceCount;

	for (int i = 0; i <= iSliceCount; i++) {
		float x = 0 * 0.5 * cos(i * dTheta);
		float z = 0 * 0.5 * sin(i * dTheta);

		v.vPos = Vec3(x, y, z);
		v.vNormal = Vec3(0, 1, 0);
		v.vTangent = Vec3(1, 0, 0);
		v.vBinormal = Vec3(0, 0, -1);
		v.vUV = Vec2(x / 1.f + 0.5f, z / 1.f + 0.5f);
		vecVtx.push_back(v);

	}

	v.vPos = Vec3(0, y, 0);
	v.vNormal = Vec3(0, 1, 0);
	v.vTangent = Vec3(1, 0, 0);
	v.vBinormal = Vec3(0, 0, -1);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	centerIndex = static_cast<int>(vecVtx.size()) - 1;

	for (int i = 0; i < iSliceCount; i++) {
		vecIdx.push_back(centerIndex);
		vecIdx.push_back(baseIndex + i + 1);
		vecIdx.push_back(baseIndex + i);
	}

	//bottomcap
	baseIndex = static_cast<int>(vecVtx.size());

	y = -0.5f;
	dTheta = 2.0f * XM_PI / iSliceCount;

	for (int i = 0; i <= iSliceCount; i++) {

		float x = 0.5 * cos(i * dTheta);
		float z = 0.5 * sin(i * dTheta);

		v.vPos = Vec3(x, y, z);
		v.vNormal = Vec3(0, -1, 0);
		v.vTangent = Vec3(1, 0, 0);
		v.vBinormal = Vec3(0, 0, 1);
		v.vUV = Vec2(x / 1.f + 0.5f, z / 1.f + 0.5f);
		vecVtx.push_back(v);

	}

	v.vPos = Vec3(0, y, 0);
	v.vNormal = Vec3(0, -1, 0);
	v.vTangent = Vec3(1, 0, 0);
	v.vBinormal = Vec3(0, 0, 1);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);


	centerIndex = static_cast<int>(vecVtx.size()) - 1;

	for (int i = 0; i < iSliceCount; i++) {
		vecIdx.push_back(centerIndex);
		vecIdx.push_back(baseIndex + i);
		vecIdx.push_back(baseIndex + i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"ConeMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	/////////////////
	//IceCream Mesh//
	/////////////////

	stackHeight = 0.5 / iStackCount;
	radiusStep = 0.5f / iStackCount;
	ringCount = iStackCount + 1;

	for (int i = 0; i < ringCount; i++) {
		float y = i * stackHeight;
		float r = i * radiusStep;
		float dTheta = 2.0f * XM_PI / iSliceCount;
		for (int j = 0; j <= iSliceCount; j++) {
			float c = cos(j * dTheta);
			float s = sin(j * dTheta);

			v.vPos = Vec3(r * c, r * s, y);
			v.vUV = Vec2(static_cast<float>(j) / iSliceCount, 1.0f - static_cast<float>(i) / iStackCount);

			v.vNormal = Vec3(c, s, 0);
			v.vNormal = v.vNormal.Normalize();
			v.vTangent = Vec3(c, -s, 0);
			v.vTangent = v.vTangent.Normalize();
			v.vBinormal.Cross(v.vNormal, v.vTangent);

			vecVtx.push_back(v);
		}
	}

	ringVertexCount = iSliceCount + 1;
	for (int i = 0; i < iStackCount; i++) {
		for (int j = 0; j < iSliceCount; j++) {
			vecIdx.push_back(i * ringVertexCount + j);
			vecIdx.push_back((i + 1) * ringVertexCount + j + 1);
			vecIdx.push_back((i + 1) * ringVertexCount + j);

			vecIdx.push_back(i * ringVertexCount + j);
			vecIdx.push_back((i)*ringVertexCount + j + 1);
			vecIdx.push_back((i + 1) * ringVertexCount + j + 1);
		}
	}

	//bottomcap
	baseIndex = static_cast<int>(vecVtx.size());

	y = 0.5f;
	dTheta = 2.0f * XM_PI / iSliceCount;

	for (int i = 0; i <= iSliceCount; i++) {

		float x = 0.5 * cos(i * dTheta);
		float z = 0.5 * sin(i * dTheta);

		v.vPos = Vec3(x, z, y);
		v.vNormal = Vec3(0, 0, 1);
		v.vTangent = Vec3(-1, 0, 0);
		v.vBinormal = Vec3(0, 1, 0);
		v.vUV = Vec2(x / 1.f + 0.5f, z / 1.f + 0.5f);
		vecVtx.push_back(v);

	}

	v.vPos = Vec3(0, 0, y);
	v.vNormal = Vec3(0, 0, 1);
	v.vTangent = Vec3(1, 0, 0);
	v.vBinormal = Vec3(0, 1, 0);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);


	centerIndex = static_cast<int>(vecVtx.size()) - 1;

	for (int i = 0; i < iSliceCount; i++) {
		vecIdx.push_back(centerIndex);
		vecIdx.push_back(baseIndex + i);
		vecIdx.push_back(baseIndex + i + 1);
	}


	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"IceCreamMesh", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}


void CResMgr::CreateDefaultGraphicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "TANGENT", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "NORMAL", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "BINORMAL", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "BLENDWEIGHT", 0, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "BLENDINDICES", 0, 0);

	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 1);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 2);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WORLD", 1, 3);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 1);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 2);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WV", 1, 3);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 0);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 1);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 2);
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "WVP", 1, 3);
	AddInputLayout(DXGI_FORMAT_R32_UINT, "ROWINDEX", 1, 0);

	Ptr<CGraphicsShader> pShader = nullptr;

	// ============================
	// Std2DShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// StdMoveMouseShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"StdMoveMouseShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Std2DUIShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DUIShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_OPAQUE);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Std2DUITransShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DUITransShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_TRANSPARENT);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// MouseCursorShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"MouseCursorShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_TRANSPARENT);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// CharacterPanelShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"CharacterPanelShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// TransMaskUIShader
	// RasterizerState      : CULL_NONE
	// BlendState           : MASK
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TransMaskUIShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// BlendMaskShader
	// RasterizerState      : CULL_NONE
	// BlendState           : MASK
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"BlendMaskShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// CoolDownShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
		pShader = new CGraphicsShader;
	pShader->SetKey(L"CoolDownShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_CoolDown");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_CoolDown");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// EXPRatioShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"EXPRatioShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_EXPRatio");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_EXPRatio");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// HPMPRatioShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"HPMPRatioShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_HPMPRatio");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_HPMPRatio");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// BarShader  //한가지만 처리
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"BarRatioShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_BarRatioShader");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_BarRatioShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_MASK);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	pShader->AddScalarParam(FLOAT_0, "Ratio      ");
	pShader->AddScalarParam(FLOAT_1, "LeftX      ");
	pShader->AddScalarParam(FLOAT_2, "RightX     ");
	pShader->AddScalarParam(FLOAT_3, "UpY        ");
	pShader->AddScalarParam(FLOAT_4, "DownY      ");
	pShader->AddScalarParam(FLOAT_5, "TotalWidth ");
	pShader->AddScalarParam(FLOAT_6, "TotalHeight");
	AddRes(pShader->GetKey(), pShader);


	// ============================
	// WorldBarShader
	// RasterizerState      : CULL_NONE
	// BlendState           : ALPHA_BLEND
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"WorldBarShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_WorldBar");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_WorldBar");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI_OPAQUE);
	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");
	AddRes(pShader->GetKey(), pShader);

	// =============================
	// MiniMap Shader
	// RasterRizerState   : CULL_BACK
	// BlendState :		  : MASK
	// DepthStencilState  : LESS_EQUAL
	// 
	// ==============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"MiniMapShader");
	pShader->CreateVertexShader(L"shader\\minimap.fx", "VS_MiniMapShader");
	pShader->CreatePixelShader(L"shader\\minimap.fx", "PS_MiniMapShader");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes(pShader->GetKey(), pShader);

	//	pShader = new CGraphicsShader;
	//pShader->SetKey(L"Std2DShader");
	//pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	//pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	//pShader->SetRSType(RS_TYPE::CULL_NONE);
	//pShader->SetDSType(DS_TYPE::NO_WRITE);
	//pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	//// Param
	//pShader->AddTexParam(TEX_0, "Output Texture");

	//AddRes(pShader->GetKey(), pShader);


	// ======================================
	// Std2DLightShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// g_tex_1              : Nomal Texture
	// ======================================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DLightShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2DLight");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DLight");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddScalarParam(INT_1, "Test Int");
	pShader->AddScalarParam(FLOAT_0, "Test Float");
	pShader->AddScalarParam(VEC2_0, "Test Vec2");
	pShader->AddScalarParam(VEC4_0, "Test Vec4");

	pShader->AddTexParam(TEX_0, "Output Texture 1");
	pShader->AddTexParam(TEX_1, "Output Texture 2");
	pShader->AddTexParam(TEX_2, "Output Texture 3");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Std2DAnimEditShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DAnimEditShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Anim2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Anim2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// Std3DShader
	// RasterizerState      : CULL_BACK
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std3DShader");
	pShader->CreateVertexShader(L"shader\\std3d.fx", "VS_Std3D");
	pShader->CreatePixelShader(L"shader\\std3d.fx", "PS_Std3D");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddScalarParam(INT_0, "Shading Type");
	pShader->AddTexParam(TEX_0, "Output Texture");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// Std3D_DeferredShader
	// RasterizerState      : CULL_BACK
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std3D_DeferredShader");
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_Std3D_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_Std3D_Deferred");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	// Param	
	pShader->AddTexParam(TEX_0, "Output Texture");
	pShader->AddTexParam(TEX_1, "Normal Texture");

	AddRes(pShader->GetKey(), pShader);


	// =================
	// DebugShape Shader
	// Topology : LineStrip
	// RS_TYPE  : CULL_NONE
	// DS_TYPE  : NO_TEST_NO_WRITE
	// BS_TYPE  : Default
	// g_vec4_0 : OutColor
	// ==================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DebugShapeShader");
	pShader->CreateVertexShader(L"shader\\debugshape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debugshape.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(VEC4_0, "DebugLine Color");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// DebugBounding Shader
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : Default

	// Parameter
	// g_float_0 : Bounding
	// 
	// Domain : DOMAIN_MASK
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DebugBoundingShader");
	pShader->CreateVertexShader(L"shader\\debugbounding.fx", "VS_DebugBounding");
	pShader->CreateGeometryShader(L"shader\\debugbounding.fx", "GS_DebugBounding");
	pShader->CreatePixelShader(L"shader\\debugbounding.fx", "PS_DebugBounding");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// TileMap Shader
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : LESS
	// BS_TYPE : MASK

	// Parameter
	// g_tex_0 : Tile Atlas Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TileMapShader");
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// ParticleRender
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_WRITE
	// BS_TYPE : ALPHA_BLEND

	// Parameter
	// g_int_0 : Particle Index
	// 
	// Domain : TRANSPARENT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"ParticleRenderShader");
	pShader->CreateVertexShader(L"shader\\particle_render.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particle_render.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particle_render.fx", "PS_ParticleRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// SkyBoxShader
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_WRITE
	// BS_TYPE : ALPHA_BLEND

	// Parameter
	// g_int_0 : Particle Index
	// 
	// Domain : TRANSPARENT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"SkyBoxShader");
	pShader->CreateVertexShader(L"shader\\skybox.fx", "VS_SkyBox");
	pShader->CreatePixelShader(L"shader\\skybox.fx", "PS_SkyBox");

	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// LandScapeShader	
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT
	// 
	// Parameter
	// g_tex_0 : Output Texture
	// g_tex_1 : Normal Texture
	// Domain : Opaque
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"LandScapeShader");
	pShader->CreateVertexShader(L"shader\\landscape.fx", "VS_LandScape");
	pShader->CreateHullShader(L"shader\\landscape.fx", "HS_LandScape");
	pShader->CreateDomainShader(L"shader\\landscape.fx", "DS_LandScape");
	pShader->CreatePixelShader(L"shader\\landscape.fx", "PS_LandScape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	AddRes(pShader->GetKey(), pShader);



	// ============================
	// GrayShader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : POSTPROCESS
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"GrayShader");
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayShader");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Distortion Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : POSTPROCESS
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DistortionShader");
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	// Parameter
	pShader->AddTexParam(TEX_1, "Noise Texture");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// Directional Light Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DirLightShader");
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_DirLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_DirLightShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Point Light Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"PointLightShader");
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_PointLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_PointLightShader");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Spot Light Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"SpotLightShader");
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_SpotLightShader");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_SpotLightShader");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Light Merge Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"LightMergeShader");
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_LightMerge");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_LightMerge");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_LIGHT);
	AddRes(pShader->GetKey(), pShader);


	// ==========================
	// Decal Shader
	// RS_TYPE  : CULL_FRONT
	// DS_TYPE  : NO_TEST_NO_WRITE
	// BS_TYPE  : ALPHA_BLEND

	// g_tex_0  : Output Texture
	// g_tex_1  : Position Target
	// g_tex_2  : Data Target
	// g_vec4_0 : LightColor
	// g_int_0  : LightUse
	// g_int_1  : layerCheck
	// =========================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DecalShader");
	pShader->CreateVertexShader(L"shader\\decal.fx", "VS_Decal");
	pShader->CreatePixelShader(L"shader\\decal.fx", "PS_Decal");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DECAL);
	AddRes(pShader->GetKey(), pShader);


	// ============================
	// TessShader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_OPAQUE
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TessShader");
	pShader->CreateVertexShader(L"shader\\tess.fx", "VS_Tess");
	pShader->CreateHullShader(L"shader\\tess.fx", "HS_Tess");
	pShader->CreateDomainShader(L"shader\\tess.fx", "DS_Tess");
	pShader->CreatePixelShader(L"shader\\tess.fx", "PS_Tess");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->AddScalarParam(FLOAT_0, "Divide");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// DepthmapShader
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_UNDEFINED
	// ============================
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_DepthMap");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_DepthMap");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UNDEFINED);

	AddRes<CGraphicsShader>(L"DepthMapShader", pShader);

	// ============================
	// worldmap Shader
	// RS_TYPE : CULL_BACK
	// DS_TYPE : LESS
	// BS_TYPE : ALPHA_BLEND	 
	// Domain : DOMAIN_DEFERRED
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"worldmapShader");
	pShader->CreateVertexShader(L"shader\\worldmap.fx", "VS_worldmap_Deferred");
	pShader->CreatePixelShader(L"shader\\worldmap.fx", "PS_worldmap_Deferred");

	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEFERRED);

	// Param	
	pShader->AddTexParam(TEX_0, "Output Texture");
	pShader->AddTexParam(TEX_1, "Normal Texture");

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// ContourPaintShader
	// RasterizerState      : CULL_BACK
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"ContourPaintShader");
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_ContourPaint_Deferred");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_ContourPaint_Deferred");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::LESS); // 업스케일 오브젝트는 가려지는것까지 고려해야함
	pShader->SetBSType(BS_TYPE::MASK);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_CONTOURPAINT);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// DefaultObjWrite
	// RasterizerState      : CULL_BACK
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DefaultObjWriteShader");
	pShader->CreateVertexShader(L"shader\\std3d_deferred.fx", "VS_DefaultTexWrite");
	pShader->CreatePixelShader(L"shader\\std3d_deferred.fx", "PS_DefaultObjWrite");
	pShader->SetRSType(RS_TYPE::CULL_BACK);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE); //원본 오브젝트값을 텍스쳐에 기록할땐 원본 그대로를 기록해야하므로 다른물체에 가려지지 않도록함(가려질경우 0이 들어가게되고 테두리 연산이 잘못됨)
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_CONTOURPAINT);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Contour Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : DOMAIN_LIGHT
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"ContourShader");
	pShader->CreateVertexShader(L"shader\\light.fx", "VS_ContourMerge");
	pShader->CreatePixelShader(L"shader\\light.fx", "PS_ContourMerge");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_CONTOURPAINT);
	AddRes(pShader->GetKey(), pShader);


}


#include "CSetColorShader.h"
#include "CParticleUpdateShader.h"
#include "CHeightMapShader.h"
#include "CRaycastShader.h"
#include "CAnimation3DShader.h"
#include "CFogOfWarShader.h"
#include "CFogFilterShader.h"
#include "CCopyBoneShader.h"

void CResMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pCS = nullptr;

	// Texture 색상 변경 쉐이더
	pCS = new CSetColorShader(32, 32, 1);
	pCS->SetKey(L"SetColorCS");
	pCS->CreateComputeShader(L"shader\\setcolor.fx", "CS_SetColor");
	AddRes(pCS->GetKey(), pCS);

	// Particle Update 쉐이더
	pCS = new CParticleUpdateShader(128, 1, 1);
	pCS->SetKey(L"ParticleUpdateCS");
	pCS->CreateComputeShader(L"shader\\particle_update.fx", "CS_ParticleUpdate");
	AddRes(pCS->GetKey(), pCS);

	// Animation Matrix Update 쉐이더
	pCS = new CAnimation3DShader(256, 1, 1);
	pCS->SetKey(L"Animation3DUpdateCS");
	pCS->CreateComputeShader(L"shader\\animation3d.fx", "CS_Animation3D");
	AddRes(pCS->GetKey(), pCS);

	// 높이맵 쉐이더
	pCS = new CHeightMapShader(32, 32, 1);
	pCS->SetKey(L"HeightMapShader");
	pCS->CreateComputeShader(L"shader\\heightmap.fx", "CS_HeightMap");
	AddRes(pCS->GetKey(), pCS);
	
	// 직선 쉐이더
	pCS = new CRaycastShader(32, 32, 1);
	pCS->SetKey(L"RaycastShader");
	pCS->CreateComputeShader(L"shader\\raycast.fx", "CS_Raycast");
	AddRes(pCS->GetKey(), pCS);

	// 전장의 안개 판별 쉐이더 
	pCS = new CFogOfWarShader(16, 64, 1);
	pCS->SetKey(L"FogOfWarShader");
	pCS->CreateComputeShader(L"shader\\fogofwarshader.fx", "CS_FogOfWarShader");
	AddRes(pCS->GetKey(), pCS);

	// 전장의 안개맵 필터 쉐이더
	pCS = new CFogFilterShader(32, 32, 1);
	pCS->SetKey(L"FogFilterShader");
	pCS->CreateComputeShader(L"shader\\fogfiltershader.fx", "CS_FogFilterShader");
	AddRes(pCS->GetKey(), pCS);

	// Animation Matrix Update 쉐이더
	pCS = new CCopyBoneShader(1024, 1, 1);
	pCS->SetKey(L"CopyBoneCS");
	pCS->CreateComputeShader(L"shader\\copybone.fx", "CS_CopyBoneMatrix");
	AddRes(pCS->GetKey(), pCS);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Test Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMtrl);

	// Std2D Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);

	// Std2DUI Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DUIShader"));
	AddRes(L"Std2DUIMtrl", pMtrl);

	// Std2DUITransShader Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DUITransShader"));
	AddRes(L"Std2DUITransMtrl", pMtrl);
	// MiniMap Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"MiniMapShader"));
	AddRes(L"MiniMapMtrl", pMtrl);

	// Std2DAnim Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DAnimMtrl", pMtrl);

	// Std2DLight Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"Std2DLightMtrl", pMtrl);

	// Std2DAnimLight Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"Std2DAnimLightMtrl", pMtrl);

	// Std2DAnimEdit Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAnimEditShader"));
	AddRes(L"Std2DAnimEditMtrl", pMtrl);

	// Std3DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3DShader"));
	AddRes(L"Std3DMtrl", pMtrl);

	// Std3D_DeferredMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std3D_DeferredShader"));
	AddRes(L"Std3D_DeferredMtrl", pMtrl);

	// LandScapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"LandScapeShader"));
	AddRes(L"LandScapeMtrl", pMtrl);

	// DebugShape Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugShapeShader"));
	AddRes(L"DebugShapeMtrl", pMtrl);

	// DebugBounding Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugBoundingShader"));
	AddRes(L"DebugBoundingMtrl", pMtrl);

	// TileMap Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes(L"TileMapMtrl", pMtrl);

	// Particle Render Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderMtrl", pMtrl);

	// Skybox Material
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"SkyBoxShader"));
	AddRes(L"SkyBoxMtrl", pMtrl);

	// GrayShader(PostProcess)
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GrayShader"));
	AddRes(L"GrayMtrl", pMtrl);

	// DistortionShader(PostProcess)
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DistortionShader"));
	AddRes(L"DistortionMtrl", pMtrl);

	// DirLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DirLightShader"));
	AddRes(L"DirLightMtrl", pMtrl);

	// PointLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PointLightShader"));
	AddRes(L"PointLightMtrl", pMtrl);

	// SpotLightMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"SpotLightShader"));
	AddRes(L"SpotLightMtrl", pMtrl);

	// LightMergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"LightMergeShader"));
	AddRes(L"LightMergeMtrl", pMtrl);

	// DecalMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DecalShader"));
	AddRes(L"DecalMtrl", pMtrl);

	// TessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TessShader"));
	
	AddRes(L"TessMtrl", pMtrl);

	// DepthMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DepthMapShader"));
	AddRes<CMaterial>(L"DepthMapMtrl", pMtrl);

	// ContourMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ContourShader"));
	AddRes(L"ContourMtrl", pMtrl);

	// TurretTrailMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"TurretTrailMtrl", pMtrl);

	// TrretBlazeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"TurretBlazeMtrl", pMtrl);
}