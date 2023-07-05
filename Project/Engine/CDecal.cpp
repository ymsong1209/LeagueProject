#include "pch.h"
#include "CDecal.h"

#include "CTransform.h"
#include "CResMgr.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
	, m_DecalTex(nullptr)
	, m_Light(0)
	, m_vLightDiffuse(1.f, 1.f, 1.f, 1.f)
	, m_bShowDebug(false)
	, m_iLayerCheck(0)
	, m_BSType(BS_TYPE::ALPHA_BLEND)
{

	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"),0);
}

CDecal::CDecal(const CDecal& _other)
	: CRenderComponent(_other)
	, m_DecalTex(_other.m_DecalTex)
	, m_Light(_other.m_Light)
	, m_bShowDebug(_other.m_bShowDebug)
	, m_vLightDiffuse(_other.m_vLightDiffuse)
	, m_iLayerCheck(_other.m_iLayerCheck)
	, m_BSType(_other.m_BSType)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"),0);
}

CDecal::~CDecal()
{
}

void CDecal::finaltick()
{
	const Matrix& matWorld = Transform()->GetWorldMat();
	if (m_bShowDebug) {
		DrawDebugCube(matWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f);
	}
}

void CDecal::render()
{
	Transform()->UpdateData();

	GetMaterial(0)->GetShader()->SetBSType(m_BSType);
	GetMaterial(0)->SetScalarParam(INT_0, &m_Light);
	GetMaterial(0)->SetScalarParam(INT_1, &m_iLayerCheck);
	GetMaterial(0)->SetScalarParam(VEC4_0, &m_vLightDiffuse);
	GetMaterial(0)->SetTexParam(TEX_0, m_DecalTex);
	GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"DataTargetTex"));
	GetMaterial(0)->UpdateData();

	GetMesh()->render(0);
}

void CDecal::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	SaveResRef(m_DecalTex.Get(), _File);
	fwrite(&m_Light, sizeof(int), 1, _File);
	fwrite(&m_vLightDiffuse, sizeof(Vec4), 1, _File);
	fwrite(&m_bShowDebug, sizeof(bool), 1, _File);
	fwrite(&m_iLayerCheck, sizeof(int), 1, _File);
	fwrite(&m_BSType, sizeof(BS_TYPE), 1, _File);
}

void CDecal::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	LoadResRef(m_DecalTex, _File);
	fread(&m_Light, sizeof(int), 1, _File);
	fread(&m_vLightDiffuse, sizeof(Vec4), 1, _File);
	fread(&m_bShowDebug, sizeof(bool), 1, _File);
	fread(&m_iLayerCheck, sizeof(int), 1, _File);
	fread(&m_BSType, sizeof(BS_TYPE), 1, _File);
}