#include "pch.h"
#include "CMurkWolfMiniDeathState.h"
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniDeathState::CMurkWolfMiniDeathState()
{
}

CMurkWolfMiniDeathState::~CMurkWolfMiniDeathState()
{
}


void CMurkWolfMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf_Mini\\Death_Base");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf_Mini\\Death_Base", false, false, false, 0.f);
	CJungleDeathState::Enter();
}

void CMurkWolfMiniDeathState::tick()
{
	CJungleDeathState::tick();
}


void CMurkWolfMiniDeathState::Exit()
{
	CJungleDeathState::Exit();
}



