#include "pch.h"
#include "CRedScript.h"

#include "CRedSpawnState.h"
#include "CRedIdleState.h"
#include "CRedAlertState.h"
#include "CRedAttackState.h"
#include "CRedA2NState.h"
#include "CRedN2AState.h"
#include "CRedChaseState.h"
#include "CRedReturnState.h"
#include "CRedDeathState.h"

CRedScript::CRedScript()
	:CJungleMonsterScript((UINT)REDSCRIPT)
{
}

CRedScript::~CRedScript()
{

}
void CRedScript::begin()
{
	m_fHP = 20;
	m_fMaxHP = 20;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jungle_red");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CRedSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CRedIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CRedAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CRedA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CRedN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CRedChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CRedAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CRedReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CRedDeathState);
	CJungleMonsterScript::begin();
}

void CRedScript::tick()
{
	CJungleMonsterScript::tick();
}