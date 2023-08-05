#include "pch.h"
#include "CRazorBeakChaseState.h"
#include <Engine/CAnim3D.h>
CRazorBeakChaseState::CRazorBeakChaseState()
{
}

CRazorBeakChaseState::~CRazorBeakChaseState()
{
}


void CRazorBeakChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak\\Run", false);
}

void CRazorBeakChaseState::tick()
{
	CJungleChaseState::tick();
}

void CRazorBeakChaseState::Exit()
{
	CJungleChaseState::Exit();
}
