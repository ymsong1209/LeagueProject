#include "pch.h"
#include "CMalphiteQState.h"
#include "CChampionScript.h"
#include "CSkill.h"

CMalphiteQState::CMalphiteQState()
{
	SetName(L"Q");
}

CMalphiteQState::~CMalphiteQState()
{
}

void CMalphiteQState::tick()
{
	CChampionSkillState::tick();
}

void CMalphiteQState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* MalphiteQ = GetOwner()->GetScript<CChampionScript>()->GetSkill(1);
	MalphiteQ->SetUserObj(m_UserObj);
	MalphiteQ->SetTargetObj(m_TargetObj);
	MalphiteQ->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	MalphiteQ->Use();

	// 애니메이션
	wstring animName = L"Malphite\\Spell2";
	GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);


}

void CMalphiteQState::Exit()
{
	CChampionSkillState::Exit();
}

void CMalphiteQState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}