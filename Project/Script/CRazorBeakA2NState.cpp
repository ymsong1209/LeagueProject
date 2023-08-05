#include "pch.h"
#include "CRazorBeakA2NState.h"
#include <Engine/CAnim3D.h>

CRazorBeakA2NState::CRazorBeakA2NState()
{
}

CRazorBeakA2NState::~CRazorBeakA2NState()
{
}


void CRazorBeakA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak\\sru_razorbeak_a2n", true, 0.5f);
}

void CRazorBeakA2NState::tick()
{
	CJungleA2NState::tick();
}

void CRazorBeakA2NState::Exit()
{
	CJungleA2NState::Exit();
}

