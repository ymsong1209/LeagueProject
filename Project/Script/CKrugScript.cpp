#include "pch.h"
#include "CKrugScript.h"
#include "CKrugSpawnState.h"
#include "CKrugIdleState.h"
#include "CKrugAlertState.h"
#include "CKrugA2NState.h"
#include "CKrugN2AState.h"
#include "CKrugChaseState.h"
#include "CKrugAttackState.h"
#include "CKrugReturnState.h"
#include "CKrugDeathState.h"

CKrugScript::CKrugScript()
	:CJungleMonsterScript((UINT)KRUGSCRIPT)
{
}

CKrugScript::~CKrugScript()
{

}
void CKrugScript::begin()
{
	m_fHP = 30;
	m_fMaxHP = 30;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CKrugSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CKrugIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CKrugAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CKrugA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CKrugN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CKrugChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CKrugAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CKrugReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CKrugDeathState);
	CJungleMonsterScript::begin();
}

void CKrugScript::tick()
{
	CJungleMonsterScript::tick();
}


