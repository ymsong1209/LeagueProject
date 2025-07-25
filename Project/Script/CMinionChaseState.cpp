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

	m_fAggroTime += DT;

	//  어그로 시간이 경과하면 바로 Walk로 전환
	if (m_fAggroTime > minionScript->GetAggroTime())
	{
		minionScript->SetTargetNull();
		GetOwnerFSM()->ChangeState(L"Walk");
		return;
	}

	// 타겟이 죽었거나 유효하지 않다면 Walk로 전환
	if (!minionScript->IsTargetValid(target))
	{
		GetOwnerFSM()->ChangeState(L"Walk");
		return;
	}

	// 타겟이 사거리 범위에 있고, 공격할 수 있다면 Attack으로 전환
	if (minionScript->IsTargetInRange(target) && minionScript->CanAttack())
	{
		GetOwnerFSM()->ChangeState(L"Attack");
		return;
	}

	// 타겟이 사거리 범위에 있지만 공격할 수 없다면 아무것도 하지 않음
	if (minionScript->IsTargetInRange(target) && !minionScript->CanAttack())
	{
		return;
	}

	m_fTime += DT;

	// 첫 번째 tick에서 또는 매 0.3초마다 PathFinder를 호출
	if (m_fTime >= 0.3f || m_fTime < 0.0f)
	{
		GetOwner()->PathFinder()->FindPath(target->Transform()->GetRelativePos());
		m_fTime = 0;
	}

	minionScript->Move();

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

}

void CMinionChaseState::Exit()
{
	CUnitState::Exit();

	m_fAggroTime = 0;
	m_fTime = -1.f;

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
