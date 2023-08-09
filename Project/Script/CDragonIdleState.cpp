#include "pch.h"
#include "CDragonIdleState.h"
#include "CDragonScript.h"
#include <Engine/CAnim3D.h>

CDragonIdleState::CDragonIdleState()
{
}

CDragonIdleState::~CDragonIdleState()
{
}

void CDragonIdleState::Enter()
{
	CUnitState::Enter();
	GetOwner()->Animator3D()->PlayRepeat(L"Elder_Dragon\\sru_dragon_idle_normal", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Elder_Dragon\\sru_dragon_idle_normal", false, false, false, 0.0f);
}

void CDragonIdleState::tick()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetObjects();
	Vec3 MonAlertPos = script->GetAlertPos();
	float AlertRadius = script->GetAlertRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonAlertPos.x, 2.f) + pow(ChampPos.z - MonAlertPos.z, 2.f));
		if (distance < AlertRadius) {
			GetOwner()->Fsm()->ChangeState(L"Normal2Alert");
			break;
		}
	}
}

void CDragonIdleState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CUnitState::Exit();
}


void CDragonIdleState::HandleEvent(CGameEvent& event)
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
