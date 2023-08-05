#include "pch.h"
#include "CRazorBeakMiniDeathState.h"

CRazorBeakMiniDeathState::CRazorBeakMiniDeathState()
{
}

CRazorBeakMiniDeathState::~CRazorBeakMiniDeathState()
{
}


void CRazorBeakMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\Death_Base");
	CJungleDeathState::Enter();
}

void CRazorBeakMiniDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRazorBeakMiniDeathState::Exit()
{
}

