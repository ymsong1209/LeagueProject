#include "pch.h"
#include "CGrompAttackState.h"

CGrompAttackState::CGrompAttackState()
{
}

CGrompAttackState::~CGrompAttackState()
{
}

void CGrompAttackState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_attack1",true, 0.5f);
	CJungleAttackState::Enter();
}

void CGrompAttackState::tick()
{
	CJungleAttackState::tick();
}


void CGrompAttackState::Exit()
{
	CJungleAttackState::Exit();
}



