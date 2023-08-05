#include "pch.h"
#include "CKrugIdleState.h"
#include <Engine/CAnim3D.h>
CKrugIdleState::CKrugIdleState()
{
}

CKrugIdleState::~CKrugIdleState()
{
}

void CKrugIdleState::Enter()
{
	
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_idle_normal");
}

void CKrugIdleState::tick()
{
	CJungleIdleState::tick();
}

void CKrugIdleState::Exit()
{

	CJungleIdleState::Exit();
}

