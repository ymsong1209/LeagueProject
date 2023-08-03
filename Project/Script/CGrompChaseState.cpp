#include "pch.h"
#include "CGrompChaseState.h"
#include <Engine/CAnim3D.h>
CGrompChaseState::CGrompChaseState()
{
}

CGrompChaseState::~CGrompChaseState()
{
}


void CGrompChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"gromp\\_run", false);
}

void CGrompChaseState::tick()
{
	CJungleChaseState::tick();
}

void CGrompChaseState::Exit()
{
	CJungleChaseState::Exit();
}

