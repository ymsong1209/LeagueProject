#include "pch.h"
#include "CKrugMiniDeathState.h"

CKrugMiniDeathState::CKrugMiniDeathState()
{
}

CKrugMiniDeathState::~CKrugMiniDeathState()
{
}


void CKrugMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_death");
	CJungleDeathState::Enter();
}

void CKrugMiniDeathState::tick()
{
	CJungleDeathState::tick();
}

void CKrugMiniDeathState::Exit()
{
}
