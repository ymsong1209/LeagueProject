#include "pch.h"
#include "CSkyBox.h"

#include "CResMgr.h"
#include "CTransform.h"

#include "CGameObject.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{	
	SetType(m_Type);
	SetFrustumCheck(false);
 

}

CSkyBox::CSkyBox(const CSkyBox& _other)
	: CRenderComponent(_other)
	, m_Type(_other.m_Type)
	, m_SkyTex(_other.m_SkyTex)
{
	SetType(m_Type);
	SetFrustumCheck(false);

}


CSkyBox::~CSkyBox()
{
}

void CSkyBox::finaltick()
{

}

void CSkyBox::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	Transform()->UpdateData();


	if (SKYBOX_TYPE::SPHERE == m_Type)
	{
		GetMaterial(0)->SetTexParam(TEX_0, m_SkyTex);
	}
	else if (SKYBOX_TYPE::CUBE == m_Type)
	{
		GetMaterial(0)->SetTexParam(TEX_CUBE_0, m_SkyTex);
	}

	GetMaterial(0)->UpdateData();

	GetMesh()->render(0);
}

void CSkyBox::render(UINT _iSubset)
{
	render();
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

void CSkyBox::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CRenderComponent::SaveToLevelJsonFile(_objValue, allocator);

	_objValue.AddMember("Type", (UINT)m_Type, allocator);

	string key = "SkyTex";
	Value keyName(kStringType);
	keyName.SetString(key.c_str(), key.length(), allocator);
	_objValue.AddMember(keyName, SaveResRefJson(m_SkyTex.Get(), allocator), allocator);
}

void CSkyBox::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CRenderComponent::LoadFromLevelJsonFile(_componentValue);

	m_Type = (SKYBOX_TYPE)_componentValue["Type"].GetUint();
	LoadResRefJson(m_SkyTex, _componentValue["SkyTex"]);
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

	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyBoxMtrl"),0);
	GetMaterial(0)->SetScalarParam(INT_0, &m_Type);
}

