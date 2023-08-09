#include "pch.h"
#include "CMalphiteWState.h"

#include "CMalphiteScript.h"
#include "CSkill.h"

CMalphiteWState::CMalphiteWState()
{
	SetName(L"W");
}

CMalphiteWState::~CMalphiteWState()
{
}

void CMalphiteWState::tick()
{
	CChampionSkillState::tick();
}

void CMalphiteWState::Enter()
{
	CChampionSkillState::Enter();

	wstring animName = L"Malphite\\Spell1";
	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	CMalphiteScript* chamscript = GetOwner()->GetScript<CMalphiteScript>();
	chamscript->SetWActive(true);


	//재질을 바꿉니다.

}

void CMalphiteWState::Exit()
{
	CChampionSkillState::Exit();
}

void CMalphiteWState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
