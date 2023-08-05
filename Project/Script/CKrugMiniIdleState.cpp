#include "pch.h"
#include "CKrugMiniIdleState.h"
#include <Engine/CAnim3D.h>
CKrugMiniIdleState::CKrugMiniIdleState()
{
}

CKrugMiniIdleState::~CKrugMiniIdleState()
{
}

void CKrugMiniIdleState::Enter()
{

	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_idle_normal");
}

void CKrugMiniIdleState::tick()
{
	CJungleIdleState::tick();
}

void CKrugMiniIdleState::Exit()
{

	CJungleIdleState::Exit();
}

