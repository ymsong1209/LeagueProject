#include "pch.h"
#include "CKrugMiniReturnState.h"

CKrugMiniReturnState::CKrugMiniReturnState()
{
}

CKrugMiniReturnState::~CKrugMiniReturnState()
{
}


void CKrugMiniReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug_Mini\\krug_run2", false);
	CJungleReturnState::Enter();
}

void CKrugMiniReturnState::tick()
{
	CJungleReturnState::tick();
}

void CKrugMiniReturnState::Exit()
{
	CJungleReturnState::Exit();
}

