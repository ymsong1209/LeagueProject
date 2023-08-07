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
	MinionType Type = MinionScript->GetMinionType();

	switch (Type)
	{
	case MinionType::MELEE:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_melee\\Run", true, true, 0.1f);
	}
	break;
	case MinionType::RANGED:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_caster\\_run", true, true, 0.1f);
	}
	break;
	case MinionType::SEIGE:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_siege\\cannon_chaos_run1", true, true, 0.1f);
	}
	break;
	case MinionType::SUPER:
	{
		GetOwner()->Animator3D()->PlayRepeat(L"minion_super\\Run", true, true, 0.1f);
	}
	break;
	}

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
