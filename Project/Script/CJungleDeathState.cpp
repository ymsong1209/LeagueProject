#include "pch.h"
#include "CJungleDeathState.h"
#include <Engine/CAnim3D.h>

CJungleDeathState::CJungleDeathState()
{
}

CJungleDeathState::~CJungleDeathState()
{
}

void CJungleDeathState::Enter()
{
	
}

void CJungleDeathState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		//Á×À½ Ã³¸®
	}
}

void CJungleDeathState::Exit()
{
}


