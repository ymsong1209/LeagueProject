#include "pch.h"
#include "CJinxEState.h"
#include "CChampionScript.h"
#include "CSkill.h"

CJinxEState::CJinxEState()
{
	SetName(L"E");
}

CJinxEState::~CJinxEState()
{
}

void CJinxEState::tick()
{
	CChampionSkillState::tick();
}

void CJinxEState::Enter()
{

	CChampionSkillState::Enter();

	CSkill* JinxW = GetOwner()->GetScript<CChampionScript>()->GetSkill(3);
	JinxW->SetUserObj(m_UserObj);
	JinxW->SetTargetObj(m_TargetObj);
	JinxW->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	JinxW->Use();

	// 애니메이션
	wstring animName = L"Jinx\\Spell3";
	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);

}

void CJinxEState::Exit()
{
}

void CJinxEState::HandleEvent(CGameEvent& event)
{
}
