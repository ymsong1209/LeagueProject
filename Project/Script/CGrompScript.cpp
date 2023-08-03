#include "pch.h"
#include "CGrompScript.h"
#include "CGrompSpawnState.h"
#include "CGrompIdleState.h"
#include "CGrompAlertState.h"
#include "CGrompA2NState.h"
#include "CGrompN2AState.h"
#include "CGrompChaseState.h"
#include "CGrompAttackState.h"
#include "CGrompReturnState.h"
#include "CGrompDeathState.h"

CGrompScript::CGrompScript()
	:CJungleMonsterScript((UINT)GROMPSCRIPT)
{
	m_eJungleType = JungleType::GROMP;
}

CGrompScript::~CGrompScript()
{

}
void CGrompScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\gromp");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	GetOwner()->Fsm()->AddState(L"Spawn", new CGrompSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CGrompIdleState);
	GetOwner()->Fsm()->AddState(L"Alert", new CGrompAlertState);
	GetOwner()->Fsm()->AddState(L"A2N", new CGrompA2NState);
	GetOwner()->Fsm()->AddState(L"N2A", new CGrompN2AState);
	GetOwner()->Fsm()->AddState(L"Chase", new CGrompChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CGrompAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CGrompReturnState);
	GetOwner()->Fsm()->AddState(L"Death", new CGrompDeathState);
	CJungleMonsterScript::begin();
}

void CGrompScript::tick()
{
	CJungleMonsterScript::tick();
}


