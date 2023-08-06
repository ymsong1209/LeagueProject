#include "pch.h"
#include "CRazorBeakReturnState.h"
#include "CRazorBeakScript.h"

CRazorBeakReturnState::CRazorBeakReturnState()
{
}

CRazorBeakReturnState::~CRazorBeakReturnState()
{
}


void CRazorBeakReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak\\Run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak\\Run", true, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CRazorBeakReturnState::tick()
{
	CJungleReturnState::tick();
}

void CRazorBeakReturnState::Exit()
{
	CJungleReturnState::Exit();
}

