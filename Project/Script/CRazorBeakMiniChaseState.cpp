#include "pch.h"
#include "CRazorBeakMiniChaseState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniChaseState::CRazorBeakMiniChaseState()
{
}

CRazorBeakMiniChaseState::~CRazorBeakMiniChaseState()
{
}


void CRazorBeakMiniChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak_Mini\\Run.002", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\Run.002", true, false, false, 0.f);
}

void CRazorBeakMiniChaseState::tick()
{
	CJungleChaseState::tick();
}

void CRazorBeakMiniChaseState::Exit()
{
	CJungleChaseState::Exit();
}
