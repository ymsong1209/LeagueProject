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
	CMinionScript* minionScript = GetOwner()->GetScript<CMinionScript>(); // ��Ÿ ����
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

		// ù ��° tick���� �Ǵ� �� 0.1�ʸ��� PathFinder�� ȣ��
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

	// ��ã�� ������Ʈ�� ���� ��ΰ��� �ִٸ� Clear
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
