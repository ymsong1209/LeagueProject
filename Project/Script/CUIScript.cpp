#include "pch.h"
#include "CUIScript.h"

void CUIScript::begin()
{
	SetChampInFo(CHARACTER_TYPE::VEIN, SUMMONERS_SPELL::CLEANSE, SUMMONERS_SPELL::EXHAUST);

	m_UIBackPanel = new CGameObject;
	m_UIBackPanel->SetName(L"UIBackPanel");
	m_UIBackPanel->AddComponent(new CTransform);
	m_UIBackPanel->AddComponent(new CMeshRender);
	m_UIBackPanel->Transform()->SetRelativeScale(Vec3(135.75f, 27.75f, 0.01f));
	m_UIBackPanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIBackPanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\UIBackMaterial.mtrl"),0);
	m_UIBackPanel->MeshRender()->SetRaySightCulling(false);
	SpawnGameObject(m_UIBackPanel, Vec3(0.f, -109.5f, 373.f), 31);

	m_UIFrontPanel = new CGameObject;
	m_UIFrontPanel->SetName(L"UIFrontPanel");
	m_UIFrontPanel->AddComponent(new CTransform);
	m_UIFrontPanel->AddComponent(new CMeshRender);
	m_UIFrontPanel->Transform()->SetRelativeScale(Vec3(135.75f, 27.75f, 0.01f));
	m_UIFrontPanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIFrontPanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\UIFrontMaterial.mtrl"), 0);
	m_UIFrontPanel->MeshRender()->SetRaySightCulling(false);
	SpawnGameObject(m_UIFrontPanel, Vec3(-1.f, -110.f, 369.f), 31);
	//UI들끼리 앞뒤를 지정해주기 위해서는 깊이체크가 필요함.
}

void CUIScript::tick()
{
}

void CUIScript::BeginOverlap(CCollider2D* _Other)
{
}

void CUIScript::SaveToLevelFile(FILE* _File)
{
}

void CUIScript::LoadFromLevelFile(FILE* _FILE)
{
}

void CUIScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
}

void CUIScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
}

CUIScript::CUIScript(SCRIPT_TYPE _Type)
	:CScript((UINT)_Type)
	, m_UIBackPanel(nullptr)
	, m_UIFrontPanel(nullptr)
{
}

CUIScript::CUIScript()
	:CScript((UINT)SCRIPT_TYPE::UISCRIPT)
{
}

CUIScript::~CUIScript()
{
}
