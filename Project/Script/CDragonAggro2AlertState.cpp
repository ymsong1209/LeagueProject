#include "pch.h"
#include "CDragonAggro2AlertState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"

CDragonAggro2AlertState::CDragonAggro2AlertState()
{
}

CDragonAggro2AlertState::~CDragonAggro2AlertState()
{
}

void CDragonAggro2AlertState::Enter()
{
	CUnitState::Enter();
	wstring basename = L"Elder_Dragon\\sru_dragon_idle_ag2al";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename, true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, true, 0.5f);
	
}


void CDragonAggro2AlertState::tick()
{
	
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Alert");
		return;
	}
}



void CDragonAggro2AlertState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CUnitState::Exit();
}

void CDragonAggro2AlertState::HandleEvent(CGameEvent& event)
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

			CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
			script->GetHit(HitEvent->GetUserObj());
		}

	}

}