#include "pch.h"
#include "CJungleIdleState.h"
#include "CJungleMonsterScript.h"
#include <Engine/CAnim3D.h>

CJungleIdleState::CJungleIdleState()
{
}

CJungleIdleState::~CJungleIdleState()
{
}

void CJungleIdleState::Enter()
{
	CUnitState::Enter();
}

void CJungleIdleState::tick()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetObjects();
	Vec3 MonAggroPos = script->GetAggroPos();
	float AggroRadius = script->GetAggroRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonAggroPos.x, 2.f) + pow(ChampPos.z - MonAggroPos.z, 2.f));
		if (distance < AggroRadius) {
			GetOwner()->Fsm()->ChangeState(L"N2A");
			break;
		}
	}
}

void CJungleIdleState::Exit()
{
	CUnitState::Exit();
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}


void CJungleIdleState::HandleEvent(CGameEvent& event)
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

			CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
			script->GetHit(HitEvent->GetUserObj());
		}
	}
}
