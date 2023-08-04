#include "pch.h"
#include "CBlueChaseState.h"
#include <Engine/CAnim3D.h>
CBlueChaseState::CBlueChaseState()
{
}

CBlueChaseState::~CBlueChaseState()
{
}


void CBlueChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_blue\\Run", false);
}

void CBlueChaseState::tick()
{
	CJungleChaseState::tick();
}

void CBlueChaseState::Exit()
{
	CJungleChaseState::Exit();
}

