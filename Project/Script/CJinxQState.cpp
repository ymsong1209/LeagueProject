#include "pch.h"
#include "CJinxQState.h"
#include "CChampionScript.h"
#include "CSkill.h"

CJinxQState::CJinxQState()
{
}

CJinxQState::~CJinxQState()
{
}

void CJinxQState::tick()
{
}

void CJinxQState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxQ = GetOwner()->GetScript<CChampionScript>()->GetSkill(1);
	JinxQ->SetUserObj(m_UserObj);
	JinxQ->SetTargetObj(m_TargetObj);
	JinxQ->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	JinxQ->Use();

	// 애니메이션
	wstring animName = L"Jinx\\Spell1";
	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);
}

void CJinxQState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxQState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
