#include "pch.h"
#include "CMinionChaseState.h"
#include "CMinionScript.h"

CMinionChaseState::CMinionChaseState()
	: m_fAggroTime(0)
	, m_fTime(0)
{
}

CMinionChaseState::~CMinionChaseState()
{
}

void CMinionChaseState::tick()
{

	CGameObject* Target = GetOwner()->GetScript<CMinionScript>()->GetTarget();
	if (!GetOwner()->GetScript<CMinionScript>()->IsTargetValid(Target))
	{
		// 타겟이 이미 죽었다면 Walk로
		GetOwnerFSM()->ChangeState(L"Walk");
	}
	else
	{
		// 타겟이 살아있고 사거리 내부에 없다면 AggroTime만큼 추적한다. (Target의 현재 위치로 PathFind)
		m_fAggroTime += DT;

		if (GetOwner()->GetScript<CMinionScript>()->GetAggroTime() > m_fAggroTime)
		{
			m_fTime += DT;
			if (m_fTime >= 0.1f)
			{
				GetOwner()->PathFinder()->FindPath(Target->Transform()->GetRelativePos());
				m_fTime = 0;
			}

			GetOwner()->GetScript<CMinionScript>()->Move();

			// 사거리 내부에 들어왔다면
			if (GetOwner()->GetScript<CMinionScript>()->IsTargetInRange(Target))
			{
				// 공격 가능한 경우 Attack으로
				if (GetOwner()->GetScript<CMinionScript>()->CanAttack())
					GetOwnerFSM()->ChangeState(L"Attack");
			}
		}
		else
		{
			// AggroTime이 지나면 다시 WalkState로
			GetOwnerFSM()->ChangeState(L"Walk");
		}
	}
}

void CMinionChaseState::Enter()
{
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

	m_fAggroTime = 0;
	m_fTime = 0;
}

void CMinionChaseState::Exit()
{
	// 길찾기 컴포넌트에 남은 경로값이 있다면 Clear
	GetOwner()->PathFinder()->ClearPath();
}

void CMinionChaseState::HandleEvent(CGameEvent& event)
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
