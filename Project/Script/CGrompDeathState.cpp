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
}

void CGrompDeathState::Exit()
{
}

void CGrompDeathState::tick()
{
	CJungleDeathState::tick();
}


