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
		// Ÿ���� �̹� �׾��ٸ� Walk��
		GetOwnerFSM()->ChangeState(L"Walk");
	}
	else
	{
		// Ÿ���� ����ְ� ��Ÿ� ���ο� ���ٸ� AggroTime��ŭ �����Ѵ�. (Target�� ���� ��ġ�� PathFind)
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

			// ��Ÿ� ���ο� ���Դٸ�
			if (GetOwner()->GetScript<CMinionScript>()->IsTargetInRange(Target))
			{
				// ���� ������ ��� Attack����
				if (GetOwner()->GetScript<CMinionScript>()->CanAttack())
					GetOwnerFSM()->ChangeState(L"Attack");
			}
		}
		else
		{
			// AggroTime�� ������ �ٽ� WalkState��
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
