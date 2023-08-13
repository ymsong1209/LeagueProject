#include "pch.h"
#include "CRazorBeakN2AState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakScript.h"

CRazorBeakN2AState::CRazorBeakN2AState()
{
}

CRazorBeakN2AState::~CRazorBeakN2AState()
{
}


void CRazorBeakN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak\\sru_razorbeak_n2a", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak\\sru_razorbeak_n2a", false, false, false, 0.f);
	CJungleN2AState::Enter();
}

void CRazorBeakN2AState::tick()
{
	CJungleN2AState::tick();
}

void CRazorBeakN2AState::Exit()
{
	CJungleN2AState::Exit();
}
