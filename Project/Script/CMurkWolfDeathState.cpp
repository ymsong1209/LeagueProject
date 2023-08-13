#include "pch.h"
#include "CMurkWolfDeathState.h"
#include "CMurkWolfScript.h"
CMurkWolfDeathState::CMurkWolfDeathState()
{
}

CMurkWolfDeathState::~CMurkWolfDeathState()
{
}


void CMurkWolfDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\sru_murkwolf_death", false, false, false, 0.f);
	CJungleDeathState::Enter();
}

void CMurkWolfDeathState::tick()
{
	CJungleDeathState::tick();
}


void CMurkWolfDeathState::Exit()
{
	CJungleDeathState::Exit();
}



