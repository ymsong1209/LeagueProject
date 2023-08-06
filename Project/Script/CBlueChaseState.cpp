#include "pch.h"
#include "CBlueChaseState.h"
#include <Engine/CAnim3D.h>
#include "CBlueScript.h"
CBlueChaseState::CBlueChaseState()
{
}

CBlueChaseState::~CBlueChaseState()
{
}


void CBlueChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_blue\\Run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\Run", true, false, false, 0.f);
}

void CBlueChaseState::tick()
{
	CJungleChaseState::tick();
}

void CBlueChaseState::Exit()
{
	CJungleChaseState::Exit();
}

