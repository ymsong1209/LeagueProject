#include "pch.h"
#include "CDragonAttackState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
#include "CSkill.h"
CDragonAttackState::CDragonAttackState()
{
}

CDragonAttackState::~CDragonAttackState()
{
}

void CDragonAttackState::Enter()
{
	CUnitState::Enter();
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

		CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
		CSkill* skill = script->GetSkill(0);
		skill->SetUserObj(GetOwner());
		skill->SetTargetObj(script->GetTarget());
		skill->Use();

		Vec3 DragonPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\dragon\\attack1.mp3", 1, 0.3f, true, 120.f, DragonPos, Faction::NONE);
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
	CUnitState::Exit();
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
