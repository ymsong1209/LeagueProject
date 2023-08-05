#include "pch.h"
#include "CKrugChaseState.h"
#include <Engine/CAnim3D.h>
CKrugChaseState::CKrugChaseState()
{
}

CKrugChaseState::~CKrugChaseState()
{
}


void CKrugChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug\\krug_run2", false);
}

void CKrugChaseState::tick()
{
	CJungleChaseState::tick();
}

void CKrugChaseState::Exit()
{
	CJungleChaseState::Exit();
}

