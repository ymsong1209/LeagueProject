#include "pch.h"
#include "CGrompScript.h"
#include "CGrompSpawnState.h"
#include "CGrompIdleState.h"
#include "CGrompChaseState.h"
#include "CGrompAttackState.h"
#include "CGrompReturnState.h"
#include "CGrompDeathState.h"

CGrompScript::CGrompScript()
	:CJungleMonsterScript(JungleType::GROMP)
{
}

CGrompScript::~CGrompScript()
{

}
void CGrompScript::begin()
{
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\gromp");
	GetOwner()->Fsm()->AddState(L"Spawn", new CGrompSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CGrompIdleState);
	GetOwner()->Fsm()->AddState(L"Chase", new CGrompChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CGrompAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CGrompReturnState);
	CJungleMonsterScript::begin();
}

void CGrompScript::tick()
{
	CJungleMonsterScript::tick();
}


