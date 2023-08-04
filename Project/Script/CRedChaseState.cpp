#include "pch.h"
#include "CRedChaseState.h"
#include <Engine/CAnim3D.h>
CRedChaseState::CRedChaseState()
{
}

CRedChaseState::~CRedChaseState()
{
}


void CRedChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_red\\Run", false);
}

void CRedChaseState::tick()
{
	CJungleChaseState::tick();
}

void CRedChaseState::Exit()
{
	CJungleChaseState::Exit();
}
