#include "pch.h"
#include "CBlueA2NState.h"
#include <Engine/CAnim3D.h>

CBlueA2NState::CBlueA2NState()
{
}

CBlueA2NState::~CBlueA2NState()
{
}


void CBlueA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_blue\\sru_blue_idle_a2n", true, 0.5f);
}

void CBlueA2NState::tick()
{
	CJungleA2NState::tick();
}

void CBlueA2NState::Exit()
{
	CJungleA2NState::Exit();
}

