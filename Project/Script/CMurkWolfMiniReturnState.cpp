#include "pch.h"
#include "CMurkWolfMiniReturnState.h"

CMurkWolfMiniReturnState::CMurkWolfMiniReturnState()
{
}

CMurkWolfMiniReturnState::~CMurkWolfMiniReturnState()
{
}


void CMurkWolfMiniReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf_Mini\\Run_Base", false);
	CJungleReturnState::Enter();
}

void CMurkWolfMiniReturnState::tick()
{
	CJungleReturnState::tick();
}

void CMurkWolfMiniReturnState::Exit()
{
	CJungleReturnState::Exit();
}

