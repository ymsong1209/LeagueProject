#include "pch.h"
#include "CSkyBox.h"

#include "CResMgr.h"
#include "CTransform.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{	
	SetType(m_Type);
	//SetFrustumCheck(false);
}

CSkyBox::CSkyBox(const CSkyBox& _other)
	: CRenderComponent(_other)
	, m_Type(_other.m_Type)
	, m_SkyTex(_other.m_SkyTex)
{
	SetType(m_Type);
	//SetFrustumCheck(false);
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

void CSkyBox::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	fwrite(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
	SaveResRef(m_SkyTex.Get(), _File);
}

void CSkyBox::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	fread(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
	LoadResRef(m_SkyTex, _File);
	//생성자호출->LoadFromLevelFile순으로 함수 실행
	//생성자의 SetType에선 SkyTex가 아직 설정이 안되어있다.
	//다시 한번 재호출
	SetType(m_Type);
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
