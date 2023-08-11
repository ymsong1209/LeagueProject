#include "pch.h"
#include "CTurretIdleState.h"
#include "CUnitScript.h"

CTurretIdleState::CTurretIdleState()
{
}

CTurretIdleState::~CTurretIdleState()
{
}

void CTurretIdleState::Enter()
{
	CUnitState::Enter();
}

void CTurretIdleState::Exit()
{
	CUnitState::Exit();
}

void CTurretIdleState::HandleEvent(CGameEvent& event)
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

			// �⺻ ���ݿ��� HP ����
			if(skilltype == SkillType::BASIC_ATTACK)
				GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;
	}
}
