#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"
#include "CResMgr.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_Light(0)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"));
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
	const Matrix& matWorld = Transform()->GetWorldMat();
	//DrawDebugCube(matWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
}

void CDecal::render()
{
	Transform()->UpdateData();

	GetMaterial()->SetScalarParam(INT_0, &m_Light);
	GetMaterial()->SetTexParam(TEX_0, m_DecalTex);
	GetMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	GetMaterial()->UpdateData();

	GetMesh()->render();
}
