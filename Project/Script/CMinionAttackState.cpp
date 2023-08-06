#include "pch.h"
#include "CMinionAttackState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CMinionScript.h"
#include "CSkill.h"

CMinionAttackState::CMinionAttackState()
{
}

CMinionAttackState::~CMinionAttackState()
{
}

void CMinionAttackState::tick()
{
	CGameObject* Target = GetOwner()->GetScript<CMinionScript>()->GetTarget();

	// 타겟이 이미 죽었다면 Walk로
	if (!GetOwner()->GetScript<CMinionScript>()->IsTargetValid(Target))
	{
		GetOwnerFSM()->ChangeState(L"Walk");
	}
	else
	{
		// 타겟이 살아있지만 현재 사거리 내에 없다면
		if (!GetOwner()->GetScript<CMinionScript>()->IsTargetInRange(Target))
		{
			// Chase 상태로 전환
			GetOwnerFSM()->ChangeState(L"Chase");
		}
		else
		{
			// 타겟이 살아있고, 사거리 내에 있다면

			// 공격가능할 경우 공격
			if (GetOwner()->GetScript<CMinionScript>()->CanAttack())
			{
				CSkill* BasicAttack = GetOwner()->GetScript<CMinionScript>()->GetSkill(0);
				BasicAttack->SetUserObj(GetOwner());
				BasicAttack->SetTargetObj(Target);
				
				BasicAttack->Use();
			}

		}
	}
}

void CMinionAttackState::Enter()
{
	// 애니메이션
	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	MinionType Type = MinionScript->GetMinionType();
	switch (Type)
	{
	case MinionType::MELEE:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionmelee\\Walk");
	}
	break;
	case MinionType::RANGED:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionranged\\Walk");
	}
	break;
	case MinionType::SEIGE:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionsiege\\Walk");
	}
	break;
	case MinionType::SUPER:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionsuper\\Walk");
	}
	break;
	}

	// 공격 투사체
}

void CMinionAttackState::Exit()
{
}
