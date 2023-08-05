#include "pch.h"
#include "CRazorBeakScript.h"
#include "CRazorBeakSpawnState.h"
#include "CRazorBeakIdleState.h"
#include "CRazorBeakAlertState.h"
#include "CRazorBeakAttackState.h"
#include "CRazorBeakA2NState.h"
#include "CRazorBeakN2AState.h"
#include "CRazorBeakChaseState.h"
#include "CRazorBeakReturnState.h"
#include "CRazorBeakDeathState.h"


CRazorBeakScript::CRazorBeakScript()
	:CJungleMonsterScript((UINT)RAZORBEAKSCRIPT)
{
	m_eJungleType = JungleType::RAZORBEAK;
}

CRazorBeakScript::~CRazorBeakScript()
{

}
void CRazorBeakScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\RazorBeak");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CRazorBeakSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CRazorBeakIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CRazorBeakAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CRazorBeakA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CRazorBeakN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CRazorBeakChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CRazorBeakAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CRazorBeakReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CRazorBeakDeathState);
	CJungleMonsterScript::begin();
}

void CRazorBeakScript::tick()
{
	CJungleMonsterScript::tick();
}


