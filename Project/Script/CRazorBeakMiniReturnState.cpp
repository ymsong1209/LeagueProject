#include "pch.h"
#include "CRazorBeakMiniReturnState.h"
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniReturnState::CRazorBeakMiniReturnState()
{
}

CRazorBeakMiniReturnState::~CRazorBeakMiniReturnState()
{
}


void CRazorBeakMiniReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak_Mini\\Run.002", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\Run.002", true, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CRazorBeakMiniReturnState::tick()
{
	CJungleReturnState::tick();
}

void CRazorBeakMiniReturnState::Exit()
{
	CJungleReturnState::Exit();
}

