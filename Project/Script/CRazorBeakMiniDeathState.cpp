#include "pch.h"
#include "CRazorBeakMiniDeathState.h"
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniDeathState::CRazorBeakMiniDeathState()
{
}

CRazorBeakMiniDeathState::~CRazorBeakMiniDeathState()
{
}


void CRazorBeakMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\Death_Base");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\Death_Base", false, false, false, 0.f);
	CJungleDeathState::Enter();
}

void CRazorBeakMiniDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRazorBeakMiniDeathState::Exit()
{
}

