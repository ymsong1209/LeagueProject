#include "pch.h"
#include "CRazorBeakMiniN2AState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniN2AState::CRazorBeakMiniN2AState()
{
}

CRazorBeakMiniN2AState::~CRazorBeakMiniN2AState()
{
}


void CRazorBeakMiniN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\sru_razorbeakmini_n2a", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\sru_razorbeakmini_n2a", false, false, true, 0.5f);
	CJungleN2AState::Enter();
}

void CRazorBeakMiniN2AState::tick()
{
	CJungleN2AState::tick();
}

void CRazorBeakMiniN2AState::Exit()
{
	CJungleN2AState::Exit();
}
