#include "pch.h"
#include "CJungleChaseState.h"
#include "CJungleMonsterScript.h"

CJungleChaseState::CJungleChaseState()
{
}

CJungleChaseState::~CJungleChaseState()
{
}


void CJungleChaseState::Enter()
{

}

void CJungleChaseState::tick()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	//타겟이 설정 안되었는데 chasestate에 들어옴
	if (script->GetTarget() == nullptr)
		assert(nullptr);

	Vec3 targetpos = script->GetTarget()->Transform()->GetRelativePos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float AttackRange = script->GetAttackRange();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//공격 사거리 내에 들어옴
	if (distance < AttackRange) {
		GetOwner()->Fsm()->ChangeState(L"Attack");
	}
	else{
		GetOwner()->PathFinder()->FindPath(targetpos);
		script->PathFindMove(100.f, true);
	}

}

void CJungleChaseState::Exit()
{
	GetOwner()->Animator3D()->Reset();
}

