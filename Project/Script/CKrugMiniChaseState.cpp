#include "pch.h"
#include "CKrugMiniChaseState.h"
#include <Engine/CAnim3D.h>
CKrugMiniChaseState::CKrugMiniChaseState()
{
}

CKrugMiniChaseState::~CKrugMiniChaseState()
{
}


void CKrugMiniChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug_Mini\\krug_run2", false);
}

void CKrugMiniChaseState::tick()
{
	CJungleChaseState::tick();
}

void CKrugMiniChaseState::Exit()
{
	CJungleChaseState::Exit();
}

