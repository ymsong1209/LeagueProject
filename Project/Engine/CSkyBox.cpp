#include "pch.h"
#include "CSkyBox.h"

#include "CResMgr.h"
#include "CTransform.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{	
	SetType(m_Type);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::finaltick()
{

}

void CSkyBox::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	Transform()->UpdateData();


	if (SKYBOX_TYPE::SPHERE == m_Type)
	{
		GetMaterial()->SetTexParam(TEX_0, m_SkyTex);
	}
	else if (SKYBOX_TYPE::CUBE == m_Type)
	{
		GetMaterial()->SetTexParam(TEX_CUBE_0, m_SkyTex);
	}

	GetMaterial()->UpdateData();

	GetMesh()->render();
}

void CSkyBox::SetType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (SKYBOX_TYPE::SPHERE == _Type)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));	
	}

	else if (SKYBOX_TYPE::CUBE == m_Type)
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));				
	}

	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"));
	GetMaterial()->SetScalarParam(INT_0, &m_Type);
}
