#include "pch.h"
#include "CDragonNormal2AlertState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"

CDragonNormal2AlertState::CDragonNormal2AlertState()
	
{
}

CDragonNormal2AlertState::~CDragonNormal2AlertState()
{
}

void CDragonNormal2AlertState::Enter()
{
	CUnitState::Enter();
	GetOwner()->Animator3D()->PlayOnce(L"Elder_Dragon\\sru_dragon_idle_n2al", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Elder_Dragon\\sru_dragon_idle_n2al", false, false, true, 0.5f);
	
}

void CDragonNormal2AlertState::tick()
{

	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Alert");
		return;
	}

}

void CDragonNormal2AlertState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CUnitState::Exit();
}

void CDragonNormal2AlertState::HandleEvent(CGameEvent& event)
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
