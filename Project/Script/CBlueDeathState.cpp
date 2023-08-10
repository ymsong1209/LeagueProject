#include "pch.h"
#include "CBlueDeathState.h"
#include "CBlueScript.h"
CBlueDeathState::CBlueDeathState()
{
}

CBlueDeathState::~CBlueDeathState()
{
}


void CBlueDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_blue\\Death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\Death", false, false, false, 0.f);
	CJungleDeathState::Enter();
}

void CBlueDeathState::tick()
{
	CJungleDeathState::tick();
}

void CBlueDeathState::Exit()
{
	CJungleDeathState::Exit();
}



