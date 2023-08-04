#include "pch.h"
#include "CBlueIdleState.h"
#include <Engine/CAnim3D.h>
CBlueIdleState::CBlueIdleState()
{
}

CBlueIdleState::~CBlueIdleState()
{
}

void CBlueIdleState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_blue\\Idle1_Base",false);
}

void CBlueIdleState::tick()
{
	CJungleIdleState::tick();
}

void CBlueIdleState::Exit()
{
	CJungleIdleState::Exit();
}

