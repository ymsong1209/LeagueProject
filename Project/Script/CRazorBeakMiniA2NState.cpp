#include "pch.h"
#include "CRazorBeakMiniA2NState.h"
#include <Engine/CAnim3D.h>

CRazorBeakMiniA2NState::CRazorBeakMiniA2NState()
{
}

CRazorBeakMiniA2NState::~CRazorBeakMiniA2NState()
{
}


void CRazorBeakMiniA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\sru_razorbeakmini_a2n", true, 0.5f);
}

void CRazorBeakMiniA2NState::tick()
{
	CJungleA2NState::tick();
}

void CRazorBeakMiniA2NState::Exit()
{
	CJungleA2NState::Exit();
}

