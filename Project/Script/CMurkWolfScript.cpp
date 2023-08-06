#include "pch.h"
#include "CMurkWolfScript.h"
#include "CMurkWolfSpawnState.h"
#include "CMurkWolfIdleState.h"
#include "CMurkWolfAlertState.h"
#include "CMurkWolfAttackState.h"
#include "CMurkWolfA2NState.h"
#include "CMurkWolfN2AState.h"
#include "CMurkWolfChaseState.h"
#include "CMurkWolfReturnState.h"
#include "CMurkWolfDeathState.h"


CMurkWolfScript::CMurkWolfScript()
	:CJungleMonsterScript((UINT)MURKWOLFSCRIPT)
{
}

CMurkWolfScript::~CMurkWolfScript()
{

}
void CMurkWolfScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CMurkWolfSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CMurkWolfIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CMurkWolfAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CMurkWolfA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CMurkWolfN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CMurkWolfChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMurkWolfAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CMurkWolfReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CMurkWolfDeathState);
	CJungleMonsterScript::begin();
}

void CMurkWolfScript::tick()
{
	CJungleMonsterScript::tick();
}


