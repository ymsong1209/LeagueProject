#include "pch.h"
#include "CDragonChaseState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
CDragonChaseState::CDragonChaseState()
	: m_fTime(0.f)
{
}

CDragonChaseState::~CDragonChaseState()
{
}


void CDragonChaseState::Enter()
{
	CUnitState::Enter();
	GetOwner()->Animator3D()->PlayRepeat(L"Elder_Dragon\\sru_dragon_flying_run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Elder_Dragon\\sru_dragon_flying_run", true, false, false, 0.f);

	Vec3 DragonPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\dragon\\run.mp3", 1, 0.5f, true, 120.f, DragonPos, Faction::NONE);
}

void CDragonChaseState::tick()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	//타겟이 설정 안되었는데 chasestate에 들어옴
	if (script->GetTarget() == nullptr)
		assert(nullptr);

	Vec3 targetpos = script->GetTarget()->Transform()->GetRelativePos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float AttackRange = script->GetAttackRange();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//공격 사거리 내에 들어옴
	if (distance < AttackRange) {
		GetOwner()->Fsm()->ChangeState(L"Attack");
	}
	else {

		m_fTime += DT;
		if (m_fTime > 0.01f) {
			float targetdist = sqrt(pow(targetpos.x - m_vTargetPos.x, 2.f) + pow(targetpos.z - m_vTargetPos.z, 2.f));
			if (targetdist > 10.f) {
				GetOwner()->PathFinder()->FindPath(targetpos);
				m_vTargetPos = targetpos;
			}
			m_fTime = 0.f;
		}
		script->PathFindMove(70.f, true);
	}

	//Enter때 초기화 안해주는 이유는 공격을 하고 다시 chase로 돌아오기 때문
	m_fSoundTime += DT;
	if (m_fSoundTime > 1.f) {
		m_fSoundTime = 0.f;
		Vec3 DragonPos = GetOwner()->Transform()->GetRelativePos();
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\dragon\\run.mp3", 1, 0.5f, true, 120.f, DragonPos, Faction::NONE);
	}
}

void CDragonChaseState::Exit()
{
	CUnitState::Exit();
}

void CDragonChaseState::HandleEvent(CGameEvent& event)
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