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
		//���� ó��
	}
}

void CJungleDeathState::Exit()
{
}


