#include "pch.h"
#include "CRazorBeakMiniChaseState.h"
#include <Engine/CAnim3D.h>
CRazorBeakMiniChaseState::CRazorBeakMiniChaseState()
{
}

CRazorBeakMiniChaseState::~CRazorBeakMiniChaseState()
{
}


void CRazorBeakMiniChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak_Mini\\Run.002", false);
}

void CRazorBeakMiniChaseState::tick()
{
	CJungleChaseState::tick();
}

void CRazorBeakMiniChaseState::Exit()
{
	CJungleChaseState::Exit();
}
