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
	GetOwner()->Animator3D()->PlayRepeat(L"gromp\\_attack1",false);
}

void CGrompAttackState::Exit()
{
	CJungleAttackState::Exit();
}

void CGrompAttackState::tick()
{
	CJungleAttackState::tick();
}


