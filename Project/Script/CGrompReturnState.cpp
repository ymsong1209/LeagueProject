#include "pch.h"
#include "CGrompReturnState.h"

CGrompReturnState::CGrompReturnState()
{
}

CGrompReturnState::~CGrompReturnState()
{
}


void CGrompReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"gromp\\_run",false);
	CJungleReturnState::Enter();
}

void CGrompReturnState::tick()
{
	CJungleReturnState::tick();
}

void CGrompReturnState::Exit()
{
	CJungleReturnState::Exit();
}

