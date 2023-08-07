#include "pch.h"
#include "CDragonAlertState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"
CDragonAlertState::CDragonAlertState()
	: m_bDetectChampion(true)
	, m_bChampInAggro(false)
{
}

CDragonAlertState::~CDragonAlertState()
{
}

void CDragonAlertState::Enter()
{
	CUnitState::Enter();
	m_iAlertAnimNum = 1;
	wstring basename = L"Elder_Dragon\\sru_dragon_idle_alert";
	basename += std::to_wstring(m_iAlertAnimNum);
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.0f);
	m_bDetectChampion = true;
	m_bChampInAggro = false;
}


void CDragonAlertState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iAlertAnimNum;
		if (m_iAlertAnimNum > 3) m_iAlertAnimNum = 1;

		wstring basename = L"Elder_Dragon\\sru_dragon_idle_alert";
		basename += std::to_wstring(m_iAlertAnimNum);

		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(basename);
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.0f);
	}


	m_bDetectChampion = false;
	m_bChampInAggro = false;

	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Player")->GetObjects();
	Vec3 MonAlertPos = script->GetAlertPos();
	Vec3 MonAggroPos = script->GetAggroPos();
	float AlertRadius = script->GetAlertRange();
	float AggroRadius = script->GetAggroRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonAlertPos.x, 2.f) + pow(ChampPos.z - MonAlertPos.z, 2.f));
		// 아직 어그로 범위에 챔피언이 하나라도 있음
		if (distance < AlertRadius) {
			float dist2 = sqrt(pow(ChampPos.x - MonAggroPos.x, 2.f) + pow(ChampPos.z - MonAggroPos.z, 2.f));
			if (dist2 < AggroRadius) {
				m_bChampInAggro = true;
				break;
			}
			m_bDetectChampion = true;
		}
	}
	//챔피언 중 하나라도 aggro 범위 내에 들어옴
	if (m_bChampInAggro == true) {
		GetOwner()->Fsm()->ChangeState(L"Alert2Aggro");
		return;
	}


	//모든 champion이 어그로 범위 밖으로 나갔음
	if (m_bDetectChampion == false) {
		GetOwner()->Fsm()->ChangeState(L"Alert2Normal");
	}
}



void CDragonAlertState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	m_bDetectChampion = true;
	m_bChampInAggro = false;
	CUnitState::Exit();
}

void CDragonAlertState::HandleEvent(CGameEvent& event)
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