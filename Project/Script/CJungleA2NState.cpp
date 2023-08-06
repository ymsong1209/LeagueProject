#include "pch.h"
#include "CJungleA2NState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleA2NState::CJungleA2NState()
{
}

CJungleA2NState::~CJungleA2NState()
{
}


void CJungleA2NState::Enter()
{
}

void CJungleA2NState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
	}
}

void CJungleA2NState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}

void CJungleA2NState::HandleEvent(CGameEvent& event)
{
	if (event.GetType() == GAME_EVENT_TYPE::GET_HIT) {
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

		CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
		script->GetHit(HitEvent->GetUserObj());
	}

}

