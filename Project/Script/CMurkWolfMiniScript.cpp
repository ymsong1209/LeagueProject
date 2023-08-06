#include "pch.h"
#include "CMurkWolfMiniScript.h"
#include "CMurkWolfMiniSpawnState.h"
#include "CMurkWolfMiniIdleState.h"
#include "CMurkWolfMiniAlertState.h"
#include "CMurkWolfMiniAttackState.h"
#include "CMurkWolfMiniA2NState.h"
#include "CMurkWolfMiniN2AState.h"
#include "CMurkWolfMiniChaseState.h"
#include "CMurkWolfMiniReturnState.h"
#include "CMurkWolfMiniDeathState.h"


CMurkWolfMiniScript::CMurkWolfMiniScript()
	:CJungleMonsterScript((UINT)MURKWOLFMINISCRIPT)
{
}

CMurkWolfMiniScript::~CMurkWolfMiniScript()
{

}
void CMurkWolfMiniScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\MurkWolf_Mini");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CMurkWolfMiniSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CMurkWolfMiniIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CMurkWolfMiniAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CMurkWolfMiniA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CMurkWolfMiniN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CMurkWolfMiniChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMurkWolfMiniAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CMurkWolfMiniReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CMurkWolfMiniDeathState);
	CJungleMonsterScript::begin();
}

void CMurkWolfMiniScript::tick()
{
	CJungleMonsterScript::tick();
}


