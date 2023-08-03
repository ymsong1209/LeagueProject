#include "pch.h"
#include "CMurkWolfN2AState.h"
#include <Engine/CAnim3D.h>
CMurkWolfN2AState::CMurkWolfN2AState()
{
}

CMurkWolfN2AState::~CMurkWolfN2AState()
{
}


void CMurkWolfN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_idle_n2a", true, 0.5f);
}

void CMurkWolfN2AState::tick()
{
	CJungleN2AState::tick();
}

void CMurkWolfN2AState::Exit()
{
	CJungleN2AState::Exit();
}

