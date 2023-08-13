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
		GetOwnerFSM()->ChangeState(L"Chase");
	}
}

void CMinionAttackState::Enter()
{
	CUnitState::Enter();

	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	UnitType Type = MinionScript->GetUnitType();

	wstring animName;

	switch (Type)
	{
	case UnitType::MELEE_MINION:
	{
		animName = L"minion_melee\\Attack1";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f, 1.f);

	}
	break;
	case UnitType::RANGED_MINION:
	{
		animName = L"minion_caster\\_attack";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);

	}
	break;
	case UnitType::SIEGE_MINION:
	{
		animName = L"minion_siege\\cannon_chaos_attack1";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);

	}
	break;
	case UnitType::SUPER_MINION:
	{
		animName = L"minion_super\\Attack1";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);
	}
	break;
	}

	UINT64 targetId = MinionScript->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, false, 0.0f);

	CGameObject* Target = MinionScript->GetTarget();

	// 근거리 공격
	CSkill* BasicAttack = MinionScript->GetSkill(0);
	BasicAttack->SetUserObj(GetOwner());
	BasicAttack->SetTargetObj(Target);
	BasicAttack->Use();
	MinionScript->ResetAttackCoolTime();

	// 원거리 공격
}

void CMinionAttackState::Exit()
{
	CUnitState::Exit();
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

