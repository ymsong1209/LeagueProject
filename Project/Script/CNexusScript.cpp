#include "pch.h"
#include "CNexusScript.h"

#include "CNexusBrokenState.h"
#include "CNexusIdleState.h"
#include "CNexusSpawnState.h"



CNexusScript::CNexusScript()
	:CStructureScript((UINT)SCRIPT_TYPE::NEXUSSCRIPT)
	, m_vecTurrets{}
{
}

CNexusScript::~CNexusScript()
{
}

void CNexusScript::begin()
{
	CStructureScript::begin();

	// FSM에 State 추가
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	GetOwner()->Fsm()->AddState(L"Idle", new CNexusIdleState);
	GetOwner()->Fsm()->AddState(L"Broken", new CNexusBrokenState);
	GetOwner()->Fsm()->AddState(L"Spawn", new CNexusSpawnState);

	GetOwner()->Fsm()->ChangeState(L"Spawn");

	m_bUnitDead = true;
}

void CNexusScript::tick()
{
	CStructureScript::tick();

	//임시 코드
	m_vecTurrets.clear();

	// 진영이 같고 Lane이 None인 포탑(억제기 앞 포탑)을 포탑 벡터에 등록한다.
	vector<CGameObject*> StructureLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();

	for (int i = 0; i < StructureLayer.size(); i++)
	{
		CUnitScript* Structure = StructureLayer[i]->GetScript<CUnitScript>();
		CUnitScript* Nexus = GetOwner()->GetScript<CUnitScript>();

		if (Structure->GetFaction() == Nexus->GetFaction() && Structure->GetLane() == Lane::NONE)
		{
			if (Structure->GetUnitType() != UnitType::TURRET)
				continue;

			m_vecTurrets.push_back(StructureLayer[i]);
		}
	}

	CheckStatus();
}

void CNexusScript::CheckStatus()
{
	// 4번 포탑이 전부 Dead 상태여야 Attack 가능
	if (m_vecTurrets.size() < 2)
		return;

	// 포탑 레이어에서 모든 포탑 오브젝트를 가져와서
	// 본인 Lane의 모든 포탑이 Dead 상태인지 확인
	for (int i = 0; i < 2; i++)
	{
		CUnitScript* TurretScript = m_vecTurrets[i]->GetScript<CUnitScript>();
		if (!TurretScript->IsUnitDead())
			break;

		// 마지막 Turret까지 dead라면, dead상태 해제
		if (i == 1)
			m_bUnitDead = false;
	}
}

