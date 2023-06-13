#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CStructuredBuffer.h"



void CLandScape::init()
{
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"));

	CreateComputeShader();

	CreateTexture();

	// 레이캐스팅 결과 받는 버퍼
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true);
}

void CLandScape::CreateMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	for (UINT i = 0; i < m_iFaceZ + 1; ++i)
	{
		for (UINT j = 0; j < m_iFaceX + 1; ++j)
		{
			v.vPos = Vec3((float)j, 0.f, (float)i);
			v.vUV = Vec2((float)j, (float)m_iFaceZ - i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	vector<UINT> vecIdx;

	for (UINT i = 0; i < m_iFaceZ; ++i)
	{
		for (UINT j = 0; j < m_iFaceX; ++j)
		{
			// 0
			// | \
			// 2--1  
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j));

			// 1--2
			//  \ |
			//    0
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j + 1));
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	SetMesh(pMesh);
}

void CLandScape::CreateComputeShader()
{
	// ======================
	// 높이 수정 컴퓨트 쉐이더
	// ======================
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();

	// =====================
	// 지형 피킹 컴퓨트 쉐이더
	// =====================
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
}

void CLandScape::CreateTexture()
{
	// 높이맵 텍스쳐	
	//m_pHeightMap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\landscape\\HeightMap_01.jpg");
	m_pHeightMap = CResMgr::GetInst()->FindRes<CTexture>(L"HeightMap");
	if (!m_pHeightMap.Get()) {
		m_pHeightMap = CResMgr::GetInst()->CreateTexture(L"HeightMap"
			, 2048, 2048
			, DXGI_FORMAT_R32_FLOAT
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
			, D3D11_USAGE_DEFAULT);
	}
	

	m_pBrushTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\brush\\Brush_01.png");
}