#include "pch.h"
#include "CJinxWalkState.h"
#include <Engine/CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"
#include "CJinxScript.h"

CJinxWalkState::CJinxWalkState()
{
	SetName(L"Run");
}

CJinxWalkState::~CJinxWalkState()
{
}

void CJinxWalkState::tick()
{
	// SFX / Voice 재생
}

void CJinxWalkState::Enter()
{	
	// 애니메이션
	wstring animName = L"";
	if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::MINIGUN)
		animName = L"Jinx\\Run_Base";
	else if (GetOwner()->GetScript<CJinxScript>()->GetWeaponMode() == JinxWeaponMode::ROCKET_LAUNCHER)
		animName = L"Jinx\\jinx_rlauncher_run";

	GetOwner()->Animator3D()->PlayRepeat(animName, true, true, 0.15f);


	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, true, true, 0.15f);

	CChampionWalkState::Enter();
}

void CJinxWalkState::Exit()
{
	CChampionWalkState::Exit();
}

void CJinxWalkState::HandleEvent(CGameEvent& event)
{
	CChampionWalkState::HandleEvent(event);
}
