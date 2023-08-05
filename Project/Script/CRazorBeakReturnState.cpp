#include "pch.h"
#include "CRazorBeakReturnState.h"

CRazorBeakReturnState::CRazorBeakReturnState()
{
}

CRazorBeakReturnState::~CRazorBeakReturnState()
{
}


void CRazorBeakReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak\\Run", false);
	CJungleReturnState::Enter();
}

void CRazorBeakReturnState::tick()
{
	CJungleReturnState::tick();
}

void CRazorBeakReturnState::Exit()
{
	CJungleReturnState::Exit();
}

