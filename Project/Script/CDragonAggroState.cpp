#include "pch.h"
#include "CDragonAggroState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
CDragonAggroState::CDragonAggroState()
	: m_bDetectChampion(true)
{
}

CDragonAggroState::~CDragonAggroState()
{
}

void CDragonAggroState::Enter()
{
	CUnitState::Enter();
	m_iAggroAnimNum = 1;
	wstring basename = L"Elder_Dragon\\sru_dragon_idle_aggro";
	basename += std::to_wstring(m_iAggroAnimNum);
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.0f);
	m_bDetectChampion = true;
}


void CDragonAggroState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAggroAnimNum;
		if (m_iAggroAnimNum == 2) ++m_iAggroAnimNum;
		if (m_iAggroAnimNum > 7) m_iAggroAnimNum = 1;

		wstring basename = L"Elder_Dragon\\sru_dragon_idle_aggro";
		basename += std::to_wstring(m_iAggroAnimNum);

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.0f);
	}


	m_bDetectChampion = false;

	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
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

	//모든 champion이 어그로 범위 밖으로 나갔음
	if (m_bDetectChampion == false) {
		GetOwner()->Fsm()->ChangeState(L"Aggro2Alert");
	}
}



void CDragonAggroState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	m_bDetectChampion = true;
	CUnitState::Exit();
}

void CDragonAggroState::HandleEvent(CGameEvent& event)
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