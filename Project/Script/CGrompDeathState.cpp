#include "pch.h"
#include "CGrompDeathState.h"

CGrompDeathState::CGrompDeathState()
{
}

CGrompDeathState::~CGrompDeathState()
{
}


void CGrompDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_death");
	CJungleDeathState::Enter();
}

void CGrompDeathState::tick()
{
	CJungleDeathState::tick();
}

void CGrompDeathState::Exit()
{
}




