#include "pch.h"
#include "CJinxRState.h"
#include "CChampionScript.h"
#include "CSkill.h"
#include "CJinxScript.h"
#include <thread>
CJinxRState::CJinxRState()
{
	SetName(L"R");
}

CJinxRState::~CJinxRState()
{
}

void CJinxRState::tick()
{
	CChampionSkillState::tick();
}

void CJinxRState::Enter()
{
	CChampionSkillState::Enter();

	CSkill* JinxR = GetOwner()->GetScript<CChampionScript>()->GetSkill(4);
	JinxR->SetUserObj(m_UserObj);
	JinxR->SetTargetObj(m_TargetObj);
	JinxR->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	thread t([=]() {
		Sleep(1000);
	JinxR->Use();
		});
	t.detach();


	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Spell4";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_spell4";

	GetOwner()->Animator3D()->PlayOnce(animName, true);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);
}

void CJinxRState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxRState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
