#include "pch.h"
#include "CBlueDeathState.h"

CBlueDeathState::CBlueDeathState()
{
}

CBlueDeathState::~CBlueDeathState()
{
}


void CBlueDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_blue\\Death");
	CJungleDeathState::Enter();
}

void CBlueDeathState::tick()
{
	CJungleDeathState::tick();
}

void CBlueDeathState::Exit()
{
}



