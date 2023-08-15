#include "pch.h"
#include "CUIScript.h"
#include "CUnitScript.h"

void CUIScript::begin()
{
	CUnitScript* UnitInfo = CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>();
	SetChampInFo(UnitInfo->GetChampType(), SUMMONERS_SPELL::HEAL, SUMMONERS_SPELL::FLASH);

	m_UIBackPanel = new CGameObject;
	m_UIBackPanel->SetName(L"UIBackPanel");
	m_UIBackPanel->AddComponent(new CTransform);
	m_UIBackPanel->AddComponent(new CMeshRender);
	m_UIBackPanel->Transform()->SetRelativeScale(Vec3(616.545f, 126.715f, 0.009f));
	m_UIBackPanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIBackPanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\UIBackMaterial.mtrl"),0);
	m_UIBackPanel->MeshRender()->SetRaySightCulling(false);
	SpawnGameObject(m_UIBackPanel, Vec3(0.f, -446.805f, 353.145f), 31);

	m_UIFrontPanel = new CGameObject;
	m_UIFrontPanel->SetName(L"UIFrontPanel");
	m_UIFrontPanel->AddComponent(new CTransform);
	m_UIFrontPanel->AddComponent(new CMeshRender);
	m_UIFrontPanel->Transform()->SetRelativeScale(Vec3(616.545f, 126.715f, 0.01f));
	m_UIFrontPanel->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_UIFrontPanel->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\UIFrontMaterial.mtrl"), 0);
	m_UIFrontPanel->MeshRender()->SetRaySightCulling(false);
	SpawnGameObject(m_UIFrontPanel, Vec3(0.f, -446.805f, 333.f), 31);
	//UI들끼리 앞뒤를 지정해주기 위해서는 깊이체크가 필요함.
}

void CUIScript::tick()
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
