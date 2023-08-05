#include "pch.h"
#include "CRazorBeakDeathState.h"

CRazorBeakDeathState::CRazorBeakDeathState()
{
}

CRazorBeakDeathState::~CRazorBeakDeathState()
{
}


void CRazorBeakDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak\\Death_Base");
	CJungleDeathState::Enter();
}

void CRazorBeakDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRazorBeakDeathState::Exit()
{
}

