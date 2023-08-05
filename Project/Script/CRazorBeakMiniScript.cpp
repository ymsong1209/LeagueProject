#include "pch.h"
#include "CRazorBeakMiniScript.h"

#include "CRazorBeakMiniSpawnState.h"
#include "CRazorBeakMiniIdleState.h"
#include "CRazorBeakMiniAlertState.h"
#include "CRazorBeakMiniAttackState.h"
#include "CRazorBeakMiniA2NState.h"
#include "CRazorBeakMiniN2AState.h"
#include "CRazorBeakMiniChaseState.h"
#include "CRazorBeakMiniReturnState.h"
#include "CRazorBeakMiniDeathState.h"

CRazorBeakMiniScript::CRazorBeakMiniScript()
	:CJungleMonsterScript((UINT)RAZORBEAKMINISCRIPT)
{
	m_eJungleType = JungleType::RAZORBEAK_MINI;
}

CRazorBeakMiniScript::~CRazorBeakMiniScript()
{

}
void CRazorBeakMiniScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak_Mini");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CRazorBeakMiniSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CRazorBeakMiniIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CRazorBeakMiniAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CRazorBeakMiniA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CRazorBeakMiniN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CRazorBeakMiniChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CRazorBeakMiniAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CRazorBeakMiniReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CRazorBeakMiniDeathState);
	CJungleMonsterScript::begin();
}

void CRazorBeakMiniScript::tick()
{
	CJungleMonsterScript::tick();
}


