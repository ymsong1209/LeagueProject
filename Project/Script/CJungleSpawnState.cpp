#include "pch.h"
#include "CJungleSpawnState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleSpawnState::CJungleSpawnState()
{
}

CJungleSpawnState::~CJungleSpawnState()
{
}

void CJungleSpawnState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
	}
}

void CJungleSpawnState::Enter()
{
	CUnitState::Enter();
}

void CJungleSpawnState::Exit()
{
	CUnitState::Exit();
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	script->SetSpawnPos(CurPos);
	Vec3 CurRot = GetOwner()->Transform()->GetRelativeRot();
	script->SetSpawnRot(CurRot);
}
