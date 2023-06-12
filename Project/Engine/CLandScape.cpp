#include "pch.h"
#include "CLandScape.h"

#include "CResMgr.h"
#include "CTransform.h"

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iFaceX(0)
	, m_iFaceZ(0)
{	
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandScapeMtrl"));
}

CLandScape::~CLandScape()
{
}


void CLandScape::finaltick()
{
}

void CLandScape::render()
{
	if (GetMesh() == nullptr || GetMaterial() == nullptr)
		return;

	Transform()->UpdateData();

	GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	GetMaterial()->SetScalarParam(INT_0, &m_iFaceX);
	GetMaterial()->SetScalarParam(INT_1, &m_iFaceZ);
	GetMaterial()->SetTexParam(TEX_2, m_HeightMap);

	GetMaterial()->UpdateData();

	GetMesh()->render();	
}

void CLandScape::SetFace(UINT _iFaceX, UINT _iFaceZ)
{
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceZ;


	CreateMesh();
}

void CLandScape::CreateMesh()
{
	Vtx v;
	vector<Vtx> vecVtx;

	for (int i = 0; i < m_iFaceZ + 1; ++i)
	{
		for (int j = 0; j < m_iFaceX + 1; ++j)
		{
			v.vPos = Vec3(j, 0.f, i);
			v.vUV = Vec2((float)j, (float)m_iFaceZ - i);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, -1.f);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

			vecVtx.push_back(v);
		}
	}

	vector<UINT> vecIdx;

	for (int i = 0; i < m_iFaceZ; ++i)
	{
		for (int j = 0; j < m_iFaceX; ++j)
		{
			// 0
			// | \
			// 2--1  
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j));

			// 0--1
			//  \ |
			//    2
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + (j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
		}
	}

	Ptr<CMesh> pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	SetMesh(pMesh);
}