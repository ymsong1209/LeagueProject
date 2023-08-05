#include "pch.h"
#include "CKrugDeathState.h"

CKrugDeathState::CKrugDeathState()
{
}

CKrugDeathState::~CKrugDeathState()
{
}


void CKrugDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_death");
	CJungleDeathState::Enter();
}

void CKrugDeathState::tick()
{
	CJungleDeathState::tick();
}

void CKrugDeathState::Exit()
{
}




