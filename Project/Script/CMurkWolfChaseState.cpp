#include "pch.h"
#include "CMurkWolfChaseState.h"
#include <Engine/CAnim3D.h>
CMurkWolfChaseState::CMurkWolfChaseState()
{
}

CMurkWolfChaseState::~CMurkWolfChaseState()
{
}


void CMurkWolfChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf\\sru_murkwolf_pounce", false);
}

void CMurkWolfChaseState::tick()
{
	CJungleChaseState::tick();
}

void CMurkWolfChaseState::Exit()
{
	CJungleChaseState::Exit();
}

