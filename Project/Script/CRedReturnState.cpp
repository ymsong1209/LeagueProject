#include "pch.h"
#include "CRedReturnState.h"

CRedReturnState::CRedReturnState()
{
}

CRedReturnState::~CRedReturnState()
{
}


void CRedReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_red\\Run", false);
	CJungleReturnState::Enter();
}

void CRedReturnState::tick()
{
	CJungleReturnState::tick();
}

void CRedReturnState::Exit()
{
	CJungleReturnState::Exit();
}

