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
	CMinionScript* minionScript = GetOwner()->GetScript<CMinionScript>(); // 오타 수정
	CGameObject* target = minionScript->GetTarget();

	if (!minionScript->IsTargetValid(target))
	{
		GetOwnerFSM()->ChangeState(L"Walk");
		return;
	}
	if (minionScript->IsTargetInRange(target) && !minionScript->CanAttack())
	{
		return;
	}
	if (minionScript->IsTargetInRange(target) && minionScript->CanAttack())
	{
		GetOwnerFSM()->ChangeState(L"Attack");
		return;
	}

	m_fAggroTime += DT;

	if (m_fAggroTime <= minionScript->GetAggroTime())
	{
		m_fTime += DT;

		// 첫 번째 tick에서 또는 매 0.1초마다 PathFinder를 호출
		if (m_fTime >= 0.1f || m_fTime < 0.0f)
		{
			GetOwner()->PathFinder()->FindPath(target->Transform()->GetRelativePos());
			m_fTime = 0;
		}

		minionScript->Move();

		if (minionScript->IsTargetInRange(target) && minionScript->CanAttack())
		{
			GetOwnerFSM()->ChangeState(L"Attack");
			return;
		}
	}
	else
	{
		GetOwnerFSM()->ChangeState(L"Walk");
		return;
	}
}

void CMinionChaseState::Enter()
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

	m_fAggroTime = 0.f;
	m_fTime = -1.f;
}

void CMinionChaseState::Exit()
{
	CUnitState::Exit();

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
