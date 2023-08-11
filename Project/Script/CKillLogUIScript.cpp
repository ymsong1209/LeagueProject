#include "pch.h"
#include "CKillLogUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

CKillLogUIScript::CKillLogUIScript()
	:CScript((UINT)SCRIPT_TYPE::KILLLOGUISCRIPT)
{
}

CKillLogUIScript::~CKillLogUIScript()
{
}


void CKillLogUIScript::begin()
{

}

void CKillLogUIScript::tick()
{
	killLogManager.displayTime = 3.f;
	killLogManager.shiftSpeed = 200.f;
	killLogManager.lineHeight = 75.f;

	if (KEY_TAP(KEY::U))
		killLogManager.AddKillLog(ChampionType::MALPHITE, ChampionType::JINX, Faction::BLUE);

	if (KEY_TAP(KEY::I))
		killLogManager.AddKillLog(ChampionType::MALPHITE, ChampionType::JINX, Faction::RED);

	// ų�α� ������Ʈ
	killLogManager.Update(DT);

	// UI �������� ���� �ڵ�
	for (const auto& log : killLogManager.GetKillLogs())
	{
		log.KillLogObj->Transform()->SetRelativePos(log.position);
	}
}

void CKillLogUIScript::BeginOverlap(CCollider2D* _Other)
{
}




//========ų�α� �Ŵ��� (�̱��� �ƴ�!)=========================================
void KillLogManager::SpawnChampRect(ChampionType _Killer, CGameObject* _Parent, Vec3 _Pos)
{
	CGameObject* ChampRect = new CGameObject;
	ChampRect->SetName(L"KillLogRect");
	ChampRect->AddComponent(new CTransform);
	ChampRect->AddComponent(new CMeshRender);
	ChampRect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ChampRect->Transform()->SetAbsolute(true);
	ChampRect->Transform()->SetRelativeScale(Vec3(55.f, 55.f, 1.f));
	ChampRect->Transform()->SetRelativePos(_Pos);

	switch (_Killer)
	{
	case (ChampionType::JINX):
	{
		ChampRect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\JINX_Rect.mtrl"), 0);
		_Parent->AddChild(ChampRect);
	}
	break;
	case (ChampionType::MALPHITE):
	{
		ChampRect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\malphite_Rect.mtrl"), 0);
		_Parent->AddChild(ChampRect);
	}
	break;
	}

}

void KillLogManager::AddKillLog(ChampionType _killer, ChampionType _victim, Faction _Faction)
{
	KillLog log;
	log.killer = _killer;
	log.victim = _victim;
	log.remainingTime = displayTime;
	log.initialX = initialPosition.x - 100; // ���ʿ��� ���� (100�� ���ϴ� �Ÿ��� ���� ����)
	log.position.x = log.initialX; // X ��ǥ ����

	CGameObject* SpawnKillLogObj = new CGameObject;
	SpawnKillLogObj->SetName(L"KillLogObj");
	SpawnKillLogObj->AddComponent(new CTransform);
	SpawnKillLogObj->AddComponent(new CMeshRender);
	SpawnKillLogObj->Transform()->SetRelativeScale(Vec3(194.f, 71.f, 1.f));
	SpawnKillLogObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	if (_Faction == Faction::BLUE)
		SpawnKillLogObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueKillLog.mtrl"), 0);
	else if (_Faction == Faction::RED)
		SpawnKillLogObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedKillLog.mtrl"), 0);
	SpawnKillLogObj->Transform()->SetAbsolute(true);

	SpawnChampRect(log.killer, SpawnKillLogObj,Vec3(-37.f,0.f,-20.f));
	SpawnChampRect(log.victim, SpawnKillLogObj,Vec3(60.f,0.f, -14.f));

	log.KillLogObj = SpawnKillLogObj;

	if (killLogs.empty())
	{
		log.position.y = initialPosition.y; // Y ��ǥ�� �ʱ� ��ġ�� ����
		log.targetY = initialPosition.y; // Target position ����
	}
	else
	{
		log.position.y = initialPosition.y + lineHeight * killLogs.size(); // ���� ų�α��� ��ǥ ��ġ���� lineHeight��ŭ ���� ��ġ
		log.targetY = log.position.y; // Target position ����
	}
	log.position.z = initialPosition.z; // Z ��ǥ �ʱ� ��ġ ����

	killLogs.push_back(log); // ���ʿ� �߰�
	needToShift = true;
	UpdateTargetPositions();
	SpawnGameObject(SpawnKillLogObj, Vec3(log.position.x, log.position.y, 150.f), 31);
}
