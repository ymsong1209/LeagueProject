#include "pch.h"
#include "CJungleReturnState.h"
#include "CJungleMonsterScript.h"
CJungleReturnState::CJungleReturnState()
{
}

CJungleReturnState::~CJungleReturnState()
{
}


void CJungleReturnState::Enter()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	Vec3 SpawnPos = script->GetSpawnPos();
	GetOwner()->PathFinder()->FindPath(SpawnPos);
	float MaxHP = script->GetMaxHP();
	script->SetCurHP(MaxHP);
}

void CJungleReturnState::tick()
{

	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();

	float MaxHP = script->GetMaxHP();
	script->SetCurHP(MaxHP);

	Vec3 targetpos = script->GetSpawnPos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//Spawn범위 근처에 옴
	if (distance < 3.f) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
	}
	else {
		//GetOwner()->PathFinder()->FindPath(targetpos);
		script->PathFindMove(100.f, true);
	}
}

void CJungleReturnState::Exit()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	GetOwner()->Transform()->SetRelativePos(script->GetSpawnPos());
	GetOwner()->Transform()->SetRelativeRot(script->GetSpawnRot());
}

