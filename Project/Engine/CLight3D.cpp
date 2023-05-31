#include "pch.h"
#include "CLight3D.h"

#include "CTransform.h"
#include "CRenderMgr.h"
#include "CResMgr.h"


CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightInfo{}
{
	SetLightType(LIGHT_TYPE::POINT);
}

CLight3D::~CLight3D()
{
}


void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_LightInfo.LightType = (UINT)_Type;

	if (LIGHT_TYPE::DIRECTIONAL == _Type)
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl"); 
	}

	else if (LIGHT_TYPE::POINT == _Type)
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_LightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl"); 
	}

	else
	{
		m_VolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		//m_LightMtrl = 
	}
}

void CLight3D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Transform()->SetRelativeScale(Vec3(m_LightInfo.Radius * 2.f, m_LightInfo.Radius * 2.f, m_LightInfo.Radius * 2.f));

	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);
}

void CLight3D::render()
{
	if (nullptr == m_LightMtrl || nullptr == m_VolumeMesh)
	{
		//assert(nullptr);
		return;
	}

	Transform()->UpdateData();

	m_LightMtrl->SetScalarParam(INT_0, &m_iLightIdx);
	m_LightMtrl->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex"));
	m_LightMtrl->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex"));
	m_LightMtrl->UpdateData();

	m_VolumeMesh->render();
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
}

