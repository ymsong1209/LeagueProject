#include "pch.h"
#include "CBlueScript.h"
#include "CBlueSpawnState.h"
#include "CBlueIdleState.h"
#include "CBlueAlertState.h"
#include "CBlueA2NState.h"
#include "CBlueN2AState.h"
#include "CBlueChaseState.h"
#include "CBlueAttackState.h"
#include "CBlueReturnState.h"
#include "CBlueDeathState.h"


CBlueScript::CBlueScript()
	:CJungleMonsterScript((UINT)BLUESCRIPT)
{
	m_eJungleType = JungleType::BLUE;
}

CBlueScript::~CBlueScript()
{

}
void CBlueScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_blue");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);

	GetOwner()->Fsm()->AddState(L"Spawn", new CBlueSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CBlueIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CBlueAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CBlueA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CBlueN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CBlueChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CBlueAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CBlueReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CBlueDeathState);
	CJungleMonsterScript::begin();
}

void CBlueScript::tick()
{
	CJungleMonsterScript::tick();
}

