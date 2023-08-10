#include "pch.h"
#include "CRazorBeakChaseState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakScript.h"

CRazorBeakChaseState::CRazorBeakChaseState()
{
}

CRazorBeakChaseState::~CRazorBeakChaseState()
{
}


void CRazorBeakChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"RazorBeak\\Run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak\\Run", true, false, false, 0.f);
	CJungleChaseState::Enter();
}

void CRazorBeakChaseState::tick()
{
	CJungleChaseState::tick();
}

void CRazorBeakChaseState::Exit()
{
	CJungleChaseState::Exit();
}
