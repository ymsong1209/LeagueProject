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
	CUnitState::Enter();

	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	UnitType Type = MinionScript->GetUnitType();

	wstring animName;

	switch (Type)
	{
	case UnitType::MELEE_MINION:
	{
		animName = L"minion_melee\\Run";
		GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.1f);
	}
	break;
	case UnitType::RANGED_MINION:
	{
		animName = L"minion_caster\\_run";
		GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.1f);
	}
	break;
	case UnitType::SIEGE_MINION:
	{
		animName = L"minion_siege\\cannon_chaos_run1";
		GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.1f);
	}
	break;
	case UnitType::SUPER_MINION:
	{
		animName = L"minion_super\\Run";
		GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.1f);
	}
	break;
	}

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, true, true, 0.1f);

	// ���� WayPoint�� ���� ��ã��
	Vec3 WayPoint = GetOwner()->GetScript<CMinionScript>()->GetWayPoint();
	GetOwner()->PathFinder()->FindPath(WayPoint);
}

void CMinionWalkState::Exit()
{
	CUnitState::Exit();

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
