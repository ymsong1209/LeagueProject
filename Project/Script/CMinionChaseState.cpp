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

	m_fAggroTime += DT;

	//  ��׷� �ð��� ����ϸ� �ٷ� Walk�� ��ȯ
	if (m_fAggroTime > minionScript->GetAggroTime())
	{
		minionScript->SetTargetNull();
		GetOwnerFSM()->ChangeState(L"Walk");
		return;
	}

	// Ÿ���� �׾��ų� ��ȿ���� �ʴٸ� Walk�� ��ȯ
	if (!minionScript->IsTargetValid(target))
	{
		GetOwnerFSM()->ChangeState(L"Walk");
		return;
	}

	// Ÿ���� ��Ÿ� ������ �ְ�, ������ �� �ִٸ� Attack���� ��ȯ
	if (minionScript->IsTargetInRange(target) && minionScript->CanAttack())
	{
		GetOwnerFSM()->ChangeState(L"Attack");
		return;
	}

	// Ÿ���� ��Ÿ� ������ ������ ������ �� ���ٸ� �ƹ��͵� ���� ����
	if (minionScript->IsTargetInRange(target) && !minionScript->CanAttack())
	{
		return;
	}

	m_fTime += DT;

	// ù ��° tick���� �Ǵ� �� 0.3�ʸ��� PathFinder�� ȣ��
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
