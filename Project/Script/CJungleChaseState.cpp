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
	//Ÿ���� ���� �ȵǾ��µ� chasestate�� ����
	if (script->GetTarget() == nullptr)
		assert(nullptr);

	Vec3 targetpos = script->GetTarget()->Transform()->GetRelativePos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float AttackRange = script->GetAttackRange();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//���� ��Ÿ� ���� ����
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

