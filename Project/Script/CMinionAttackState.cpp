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
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
	{
		CGameObject* Target = GetOwner()->GetScript<CMinionScript>()->GetTarget();

		// 공격
		CSkill* BasicAttack = GetOwner()->GetScript<CMinionScript>()->GetSkill(0);
		BasicAttack->SetUserObj(GetOwner());
		BasicAttack->SetTargetObj(Target);

		BasicAttack->Use();
		GetOwner()->GetScript<CMinionScript>()->ResetAttackCoolTime();

		GetOwnerFSM()->ChangeState(L"Chase");
	}
}

void CMinionAttackState::Enter()
{
	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	UnitType Type = MinionScript->GetUnitType();

	switch (Type)
	{
	case UnitType::MELEE_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_melee\\Attack1", true, 0.1f);
	}
	break;
	case UnitType::RANGED_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_caster\\_attack", true, 0.1f);
	}
	break;
	case UnitType::SIEGE_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_siege\\cannon_chaos_attack1", true, 0.1f);
	}
	break;
	case UnitType::SUPER_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_super\\Attack1", true, 0.1f);
	}
	break;
	}
}

void CMinionAttackState::Exit()
{

}

void CMinionAttackState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::GET_HIT:
	{
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// 맞은 타겟이 본인인 경우에만 이벤트에 반응
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;
	}

}

