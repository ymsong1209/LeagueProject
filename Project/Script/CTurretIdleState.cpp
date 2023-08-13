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

		// 맞은 타겟이 본인인 경우에만 이벤트에 반응
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			// 기본 공격에만 HP 감소
			if(skilltype == SkillType::BASIC_ATTACK)
				GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;
	}
}
