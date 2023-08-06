#include "pch.h"
#include "CKrugMiniScript.h"
#include "CKrugMiniSpawnState.h"
#include "CKrugMiniIdleState.h"
#include "CKrugMiniAlertState.h"
#include "CKrugMiniA2NState.h"
#include "CKrugMiniChaseState.h"
#include "CKrugMiniAttackState.h"
#include "CKrugMiniReturnState.h"
#include "CKrugMiniDeathState.h"

CKrugMiniScript::CKrugMiniScript()
	:CJungleMonsterScript((UINT)KRUGMINISCRIPT)
{
}

CKrugMiniScript::~CKrugMiniScript()
{

}
void CKrugMiniScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Krug_Mini");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CKrugMiniSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CKrugMiniIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CKrugMiniAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CKrugMiniA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CKrugMiniA2NState);
	GetOwner()->Fsm()->AddState(L"Chase", new CKrugMiniChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CKrugMiniAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CKrugMiniReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CKrugMiniDeathState);
	CJungleMonsterScript::begin();
}

void CKrugMiniScript::tick()
{
	CJungleMonsterScript::tick();
}
