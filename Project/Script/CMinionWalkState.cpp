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
	// Waypoint�� ���� Move
	GetOwner()->GetScript<CMinionScript>()->Move();

	// ���� Ÿ���� �ִٸ�
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

	// ���� WayPoint�� ���� ��ã��
	Vec3 WayPoint = GetOwner()->GetScript<CMinionScript>()->GetWayPoint();
	GetOwner()->PathFinder()->FindPath(WayPoint);
}

void CMinionWalkState::Exit()
{
	// ��ã�� ������Ʈ�� ���� ��ΰ��� �ִٸ� Clear
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

		// ���� Ÿ���� ������ ��쿡�� �̺�Ʈ�� ����
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
