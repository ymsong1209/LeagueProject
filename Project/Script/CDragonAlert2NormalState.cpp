#include "pch.h"
#include "CDragonAlert2NormalState.h"
#include <Engine/CAnim3D.h>
#include "CDragonScript.h"

CDragonAlert2NormalState::CDragonAlert2NormalState()
{
}

CDragonAlert2NormalState::~CDragonAlert2NormalState()
{
}

void CDragonAlert2NormalState::Enter()
{
	CUnitState::Enter();
	wstring basename = L"Elder_Dragon\\sru_dragon_idle_al2n";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename, true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, true, 0.5f);

}


void CDragonAlert2NormalState::tick()
{

	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
		return;
	}
}



void CDragonAlert2NormalState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CUnitState::Exit();
}

void CDragonAlert2NormalState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	if (event.GetType() == GAME_EVENT_TYPE::GET_HIT) {
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// ���� Ÿ���� ������ ��쿡�� �̺�Ʈ�� ����
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