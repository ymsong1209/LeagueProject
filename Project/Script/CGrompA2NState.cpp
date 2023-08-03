#include "pch.h"
#include "CGrompA2NState.h"
#include <Engine/CAnim3D.h>

CGrompA2NState::CGrompA2NState()
{
}

CGrompA2NState::~CGrompA2NState()
{
}


void CGrompA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1_a2n", true, 0.5f);
}

void CGrompA2NState::tick()
{
	CJungleA2NState::tick();
}

void CGrompA2NState::Exit()
{
	CJungleA2NState::Exit();
}

