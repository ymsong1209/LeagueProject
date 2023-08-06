#include "pch.h"
#include "CRedDeathState.h"
#include "CRedScript.h"

CRedDeathState::CRedDeathState()
{
}

CRedDeathState::~CRedDeathState()
{
}


void CRedDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\Death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_red\\Death", false, false, false, 0.f);
	CJungleDeathState::Enter();
}

void CRedDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRedDeathState::Exit()
{
}

