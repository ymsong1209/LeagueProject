#include "pch.h"
#include "CJungleN2AState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleN2AState::CJungleN2AState()
	: m_bDetectChampion(true)
{
}

CJungleN2AState::~CJungleN2AState()
{
}

void CJungleN2AState::Enter()
{
	CUnitState::Enter();
	m_bDetectChampion = true;
}

void CJungleN2AState::tick()
{
	m_bDetectChampion = false;
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Alert");
		return;
	}

	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetObjects();
	Vec3 MonAggroPos = script->GetAggroPos();
	float AggroRadius = script->GetAggroRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonAggroPos.x, 2.f) + pow(ChampPos.z - MonAggroPos.z, 2.f));
		// 아직 어그로 범위에 챔피언이 하나라도 있음
		if (distance < AggroRadius) {
			m_bDetectChampion = true;
			break;
		}
	}
	//Animation 재생 중에 모든 champion이 어그로 범위 밖으로 나갔음
	if (m_bDetectChampion == false) {
		GetOwner()->Fsm()->ChangeState(L"A2N");
	}

	
}

void CJungleN2AState::Exit()
{
	CUnitState::Exit();
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	m_bDetectChampion = true;
}

void CJungleN2AState::HandleEvent(CGameEvent& event)
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

		CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
		script->GetHit(HitEvent->GetUserObj());
	}

}
