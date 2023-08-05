#include "pch.h"
#include "CMurkWolfReturnState.h"

CMurkWolfReturnState::CMurkWolfReturnState()
{
}

CMurkWolfReturnState::~CMurkWolfReturnState()
{
}


void CMurkWolfReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf\\Run_Base", false);
	CJungleReturnState::Enter();
}

void CMurkWolfReturnState::tick()
{
	CJungleReturnState::tick();
}

void CMurkWolfReturnState::Exit()
{
	CJungleReturnState::Exit();
}

