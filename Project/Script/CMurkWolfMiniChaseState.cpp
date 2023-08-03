#include "pch.h"
#include "CMurkWolfMiniChaseState.h"
#include <Engine/CAnim3D.h>
CMurkWolfMiniChaseState::CMurkWolfMiniChaseState()
{
}

CMurkWolfMiniChaseState::~CMurkWolfMiniChaseState()
{
}


void CMurkWolfMiniChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf_Mini\\sru_murkwolf_pounce", false);
}

void CMurkWolfMiniChaseState::tick()
{
	CJungleChaseState::tick();
}

void CMurkWolfMiniChaseState::Exit()
{
	CJungleChaseState::Exit();
}
