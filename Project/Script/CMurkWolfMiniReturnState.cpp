#include "pch.h"
#include "CMurkWolfMiniReturnState.h"
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniReturnState::CMurkWolfMiniReturnState()
{
}

CMurkWolfMiniReturnState::~CMurkWolfMiniReturnState()
{
}


void CMurkWolfMiniReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf_Mini\\Run_Base", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf_Mini\\Run_Base", true, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CMurkWolfMiniReturnState::tick()
{
	CJungleReturnState::tick();
}

void CMurkWolfMiniReturnState::Exit()
{
	CJungleReturnState::Exit();
}

