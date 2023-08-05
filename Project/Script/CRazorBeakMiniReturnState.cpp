#include "pch.h"
#include "CRazorBeakMiniReturnState.h"

CRazorBeakMiniReturnState::CRazorBeakMiniReturnState()
{
}

CRazorBeakMiniReturnState::~CRazorBeakMiniReturnState()
{
}


void CRazorBeakMiniReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak_Mini\\Run.002", false);
	CJungleReturnState::Enter();
}

void CRazorBeakMiniReturnState::tick()
{
	CJungleReturnState::tick();
}

void CRazorBeakMiniReturnState::Exit()
{
	CJungleReturnState::Exit();
}

