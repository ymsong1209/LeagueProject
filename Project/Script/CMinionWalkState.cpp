#include "pch.h"
#include "CMinionWalkState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CMinionScript.h"

CMinionWalkState::CMinionWalkState()
{
}

CMinionWalkState::~CMinionWalkState()
{
}

void CMinionWalkState::tick()
{
	// Waypoint를 향해 Move
	GetOwner()->GetScript<CMinionScript>()->Move();

	// 공격 타겟이 있다면
	if (GetOwner()->GetScript<CMinionScript>()->GetTarget())
		GetOwnerFSM()->ChangeState(L"Chase");
}

void CMinionWalkState::Enter()
{
	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	UnitType Type = MinionScript->GetUnitType();

	switch (Type)
	{
	case UnitType::MELEE_MINION:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_melee\\Run", true, true, 0.1f);
	}
	break;
	case UnitType::RANGED_MINION:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_caster\\_run", true, true, 0.1f);
	}
	break;
	case UnitType::SIEGE_MINION:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_siege\\cannon_chaos_run1", true, true, 0.1f);
	}
	break;
	case UnitType::SUPER_MINION:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_super\\Run", true, true, 0.1f);
	}
	break;
	}

	// 다음 WayPoint를 향해 길찾기
	Vec3 WayPoint = GetOwner()->GetScript<CMinionScript>()->GetWayPoint();
	GetOwner()->PathFinder()->FindPath(WayPoint);
}

void CMinionWalkState::Exit()
{
	// 길찾기 컴포넌트에 남은 경로값이 있다면 Clear
	GetOwner()->PathFinder()->ClearPath();
}

void CMinionWalkState::HandleEvent(CGameEvent& event)
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
