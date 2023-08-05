#include "pch.h"
#include "CRedDeathState.h"

CRedDeathState::CRedDeathState()
{
}

CRedDeathState::~CRedDeathState()
{
}


void CRedDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\Death");
	CJungleDeathState::Enter();
}

void CRedDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRedDeathState::Exit()
{
}

