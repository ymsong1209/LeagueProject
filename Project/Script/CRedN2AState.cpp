#include "pch.h"
#include "CRedN2AState.h"
#include <Engine/CAnim3D.h>
CRedN2AState::CRedN2AState()
{
}

CRedN2AState::~CRedN2AState()
{
}


void CRedN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\sru_red_idle_n2a", true, 0.5f);
}

void CRedN2AState::tick()
{
	CJungleN2AState::tick();
}

void CRedN2AState::Exit()
{
	CJungleN2AState::Exit();
}