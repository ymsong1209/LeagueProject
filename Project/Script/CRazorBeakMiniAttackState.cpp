#include "pch.h"
#include "CRazorBeakMiniAttackState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

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
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, basename, false, false, false, 0.f);
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
