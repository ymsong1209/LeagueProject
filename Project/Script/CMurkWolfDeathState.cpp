#include "pch.h"
#include "CMurkWolfDeathState.h"

CMurkWolfDeathState::CMurkWolfDeathState()
{
}

CMurkWolfDeathState::~CMurkWolfDeathState()
{
}


void CMurkWolfDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_death");
	CJungleDeathState::Enter();
}

void CMurkWolfDeathState::tick()
{
	CJungleDeathState::tick();
}


void CMurkWolfDeathState::Exit()
{
}



