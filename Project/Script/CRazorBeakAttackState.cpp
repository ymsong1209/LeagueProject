#include "pch.h"
#include "CRazorBeakAttackState.h"
#include <Engine/CAnim3D.h>

CRazorBeakAttackState::CRazorBeakAttackState()
{
}

CRazorBeakAttackState::~CRazorBeakAttackState()
{
}

void CRazorBeakAttackState::Enter()
{
	
	wstring basename = L"RazorBeak\\Attack1";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CRazorBeakAttackState::tick()
{
	CJungleAttackState::tick();
}


void CRazorBeakAttackState::Exit()
{
	CJungleAttackState::Exit();
}
