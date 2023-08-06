#include "pch.h"
#include "CDragonAttackState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
CDragonAttackState::CDragonAttackState()
{
}

CDragonAttackState::~CDragonAttackState()
{
}

void CDragonAttackState::Enter()
{
	m_fAttackCoolTime = 0.f;
}

void CDragonAttackState::tick()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	//타겟이 설정 안되었는데 attackstate에 들어옴
	if (script->GetTarget() == nullptr)
		assert(nullptr);

	m_fAttackCoolTime -= EditorDT;
	if (m_fAttackCoolTime < 0.f) {
		// 공격 이벤트 발생
		BasicAttackEvent* evn = dynamic_cast<BasicAttackEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK));
		CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
		if (evn != nullptr)
		{
			evn->Clear();
			evn->SetUserObj(GetOwner());
			evn->SetTargetObj(script->GetTarget());

			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
		m_fAttackCoolTime = 3.f;
	}

	Vec3 targetpos = script->GetTarget()->Transform()->GetRelativePos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float AttackRange = script->GetAttackRange();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//공격 사거리에서 벗어남
	if (distance > AttackRange) {
		GetOwner()->Fsm()->ChangeState(L"Chase");
	}
}


void CDragonAttackState::Exit()
{
	m_fAttackCoolTime = 0.f;
}

void CDragonAttackState::HandleEvent(CGameEvent& event)
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
