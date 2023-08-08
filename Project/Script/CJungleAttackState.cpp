#include "pch.h"
#include "CJungleAttackState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
#include "CBasicAttack.h"
CJungleAttackState::CJungleAttackState()
{
}

CJungleAttackState::~CJungleAttackState()
{
}

void CJungleAttackState::Enter()
{
	CUnitState::Enter();
}

void CJungleAttackState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
		CSkill* skill = script->GetSkill(0);
		skill->SetUserObj(GetOwner());
		skill->SetTargetObj(script->GetTarget());
		skill->Use();


		//// 공격 이벤트 발생
		//BasicAttackEvent* evn = dynamic_cast<BasicAttackEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK));
		//
		//	if (evn != nullptr)
		//	{
		//		evn->Clear();
		//		evn->SetUserObj(GetOwner());
		//		evn->SetTargetObj(script->GetTarget());

		//		CGameEventMgr::GetInst()->NotifyEvent(*evn);
		//	}
		GetOwner()->Fsm()->ChangeState(L"Chase");
	}
}

void CJungleAttackState::Exit()
{
	CUnitState::Exit();
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}



void CJungleAttackState::HandleEvent(CGameEvent& event)
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
