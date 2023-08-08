#include "pch.h"
#include "CDragonAlert2AggroState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
CDragonAlert2AggroState::CDragonAlert2AggroState()
{
}

CDragonAlert2AggroState::~CDragonAlert2AggroState()
{
}

void CDragonAlert2AggroState::Enter()
{
	CUnitState::Enter();
	wstring basename = L"Elder_Dragon\\sru_dragon_idle_al2ag";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename, true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, true, 0.5f);
}


void CDragonAlert2AggroState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Aggro");
		return;
	}
}



void CDragonAlert2AggroState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CUnitState::Exit();
}

void CDragonAlert2AggroState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

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

		CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
		script->GetHit(HitEvent->GetUserObj());
	}

}