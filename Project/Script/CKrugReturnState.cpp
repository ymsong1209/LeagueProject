#include "pch.h"
#include "CKrugReturnState.h"

CKrugReturnState::CKrugReturnState()
{
}

CKrugReturnState::~CKrugReturnState()
{
}


void CKrugReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug\\krug_run2", false);
	CJungleReturnState::Enter();
}

void CKrugReturnState::tick()
{
	CJungleReturnState::tick();
}

void CKrugReturnState::Exit()
{
	CJungleReturnState::Exit();
}

