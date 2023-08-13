#include "pch.h"
#include "CInhibitorScript.h"

#include "CInhibitorBrokenState.h"
#include "CInhibitorIdleState.h"
#include "CInhibitorSpawnState.h"
#include "CInhibitorRespawnState.h"

CInhibitorScript::CInhibitorScript()
	:CStructureScript((UINT)SCRIPT_TYPE::INHIBITORSCRIPT)
	, m_vecTurrets{}
	, m_fRespawnTime(5)
{
	m_fMaxHP = 30.f;
}

CInhibitorScript::~CInhibitorScript()
{
	
}

void CInhibitorScript::begin()
{
	CStructureScript::begin();

	// FSM에 State 추가
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	GetOwner()->Fsm()->AddState(L"Idle", new CInhibitorIdleState);
	GetOwner()->Fsm()->AddState(L"Broken", new CInhibitorBrokenState);
	GetOwner()->Fsm()->AddState(L"Spawn", new CInhibitorSpawnState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CInhibitorRespawnState);

	GetOwner()->Fsm()->ChangeState(L"Spawn");

	// 진영 / 라인이 같은 포탑을 포탑 벡터에 등록한다.
	vector<CGameObject*> StructureLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();

	for (int i = 0; i < StructureLayer.size(); i++)
	{
		CUnitScript* Structure = StructureLayer[i]->GetScript<CUnitScript>();
		CUnitScript* Inhibitor = GetOwner()->GetScript<CUnitScript>();

		if (Structure->GetFaction() == Inhibitor->GetFaction() && Structure->GetLane() == Inhibitor->GetLane())
		{
			if (Structure == Inhibitor)
				continue;

			m_vecTurrets.push_back(StructureLayer[i]);
		}
	}

	// 공격받지 않게 m_bUnitDead처리
	m_bUnitDead = true;
}

void CInhibitorScript::tick()
{
	CheckStatus();


	//임시 코드
	m_vecTurrets.clear();

	// 진영 / 라인이 같은 포탑을 포탑 벡터에 등록한다.
	vector<CGameObject*> StructureLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();

	for (int i = 0; i < StructureLayer.size(); i++)
	{
		CUnitScript* Structure = StructureLayer[i]->GetScript<CUnitScript>();
		CUnitScript* Inhibitor = GetOwner()->GetScript<CUnitScript>();

		if (Structure->GetFaction() == Inhibitor->GetFaction() && Structure->GetLane() == Inhibitor->GetLane())
		{
			if (Structure == Inhibitor)
				continue;

			m_vecTurrets.push_back(StructureLayer[i]);
		}
	}
}

void CInhibitorScript::CheckStatus()
{
	// 공격 가능한지 여부 체크
	// 포탑이 파괴되었다면 무조건 공격 불가

	if (m_vecTurrets.size() < 3)
		return;

	if (GetOwner()->Fsm()->GetCurState()->GetName() == L"Broken"
		|| GetOwner()->Fsm()->GetCurState()->GetName() == L"Respawn")
		return;

	// 포탑 레이어에서 모든 포탑 오브젝트를 가져와서
	// 본인 Lane의 모든 포탑이 Dead 상태인지 확인
	for (int i = 0; i < 3; i++)
	{
		CUnitScript* TurretScript = m_vecTurrets[i]->GetScript<CUnitScript>();
		if (!TurretScript->IsUnitDead())
			break;

		// 마지막 Turret까지 dead라면, dead상태 해제
		if (i == 2)
			m_bUnitDead = false;
	}
}
