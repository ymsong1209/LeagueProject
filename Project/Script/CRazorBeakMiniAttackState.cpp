#include "pch.h"
#include "CRazorBeakMiniAttackState.h"
#include <Engine/CAnim3D.h>

CRazorBeakMiniAttackState::CRazorBeakMiniAttackState()
{
}

CRazorBeakMiniAttackState::~CRazorBeakMiniAttackState()
{
}

void CRazorBeakMiniAttackState::Enter()
{

	wstring basename = L"RazorBeak_Mini\\Attack1";
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	GetOwner()->Animator3D()->PlayOnce(basename);
	CJungleAttackState::Enter();
}

void CRazorBeakMiniAttackState::tick()
{
	CJungleAttackState::tick();
}


void CRazorBeakMiniAttackState::Exit()
{
	CJungleAttackState::Exit();
}
