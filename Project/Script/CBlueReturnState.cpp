#include "pch.h"
#include "CBlueReturnState.h"
CBlueReturnState::CBlueReturnState()
{
}

CBlueReturnState::~CBlueReturnState()
{
}


void CBlueReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_blue\\Run", false);
	CJungleReturnState::Enter();
}

void CBlueReturnState::tick()
{
	CJungleReturnState::tick();
}

void CBlueReturnState::Exit()
{
	CJungleReturnState::Exit();
}

