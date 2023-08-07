#include "pch.h"
#include "CBlueIdleState.h"
#include <Engine/CAnim3D.h>
#include "CBlueScript.h"
CBlueIdleState::CBlueIdleState()
{
}

CBlueIdleState::~CBlueIdleState()
{
}

void CBlueIdleState::Enter()
{
	CJungleIdleState::Enter();
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_blue\\Idle1_Base",false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\Idle1_Base", true, false, false, 0.f);
}

void CBlueIdleState::tick()
{
	CJungleIdleState::tick();
}

void CBlueIdleState::Exit()
{
	CJungleIdleState::Exit();
}

