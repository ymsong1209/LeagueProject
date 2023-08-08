#include "pch.h"
#include "CDragonLandState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
CDragonLandState::CDragonLandState()
{
}

CDragonLandState::~CDragonLandState()
{
}

void CDragonLandState::Enter()
{
	CUnitState::Enter();
	wstring basename = L"Elder_Dragon\\Landing";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename, true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, true, 0.5f);
}


void CDragonLandState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
		return;
	}
}



void CDragonLandState::Exit()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	script->ReleaseTarget();
	CUnitState::Exit();
}

void CDragonLandState::HandleEvent(CGameEvent& event)
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
	}

}