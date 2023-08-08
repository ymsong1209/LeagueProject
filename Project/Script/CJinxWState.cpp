#include "pch.h"
#include "CJinxWState.h"
#include "CChampionScript.h"
#include "CSkill.h"

CJinxWState::CJinxWState()
{
	SetName(L"W");
}

CJinxWState::~CJinxWState()
{
}

void CJinxWState::tick()
{
	CChampionSkillState::tick();
}

void CJinxWState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxW = GetOwner()->GetScript<CChampionScript>()->GetSkill(2);
	JinxW->SetUserObj(m_UserObj);
	JinxW->SetTargetObj(m_TargetObj);
	JinxW->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	JinxW->Use();

	// 애니메이션
	wstring animName = L"Jinx\\Spell2";
	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

	
}

void CJinxWState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxWState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
