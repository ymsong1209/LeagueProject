#include "pch.h"
#include "CMurkWolfReturnState.h"
#include "CMurkWolfScript.h"
CMurkWolfReturnState::CMurkWolfReturnState()
{
}

CMurkWolfReturnState::~CMurkWolfReturnState()
{
}


void CMurkWolfReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf\\Run_Base", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\Run_Base", false, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CMurkWolfReturnState::tick()
{
	CJungleReturnState::tick();
}

void CMurkWolfReturnState::Exit()
{
	CJungleReturnState::Exit();
}

