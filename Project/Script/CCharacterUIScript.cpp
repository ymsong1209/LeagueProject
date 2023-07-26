#include "pch.h"
#include "CCharacterUIScript.h"
#include "CUIScript.h"

void CCharacterUIScript::begin()
{
	CUIScript::begin();

	m_UIBackPanel = new CGameObject;
	m_UIBackPanel->SetName(L"UICharacterPanel");
	m_UIBackPanel->AddComponent(new CTransform);
	m_UIBackPanel->AddComponent(new CMeshRender);
	m_UIBackPanel->Transform()->SetRelativeScale(Vec3(135.75f, 27.75f, 0.01f));
	m_UIBackPanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIBackPanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TransChracter.mtrl"), 0);
	m_UIBackPanel->MeshRender()->GetDynamicMaterial(0);
	m_UIBackPanel->MeshRender()->SetRaySightCulling(false);
	SpawnGameObject(m_UIBackPanel, Vec3(0.f, -88.f, 373.f), 31);

	GetUIFrontPanel()->AddChild(CharacterImage);

}

void CCharacterUIScript::tick()
{
}

void CCharacterUIScript::BeginOverlap(CCollider2D* _Other)
{
}

void CCharacterUIScript::SaveToLevelFile(FILE* _File)
{
}

void CCharacterUIScript::LoadFromLevelFile(FILE* _FILE)
{
}

void CCharacterUIScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
}

void CCharacterUIScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
}

CCharacterUIScript::CCharacterUIScript()
	:CUIScript(CHARACTERUISCRIPT)
{

}


CCharacterUIScript::~CCharacterUIScript()
{
}
