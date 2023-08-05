#include "pch.h"
#include "CMurkWolfMiniDeathState.h"

CMurkWolfMiniDeathState::CMurkWolfMiniDeathState()
{
}

CMurkWolfMiniDeathState::~CMurkWolfMiniDeathState()
{
}


void CMurkWolfMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf_Mini\\Death_Base");
	CJungleDeathState::Enter();
}

void CMurkWolfMiniDeathState::tick()
{
	CJungleDeathState::tick();
}


void CMurkWolfMiniDeathState::Exit()
{
}



