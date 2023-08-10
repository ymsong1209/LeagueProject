#include "pch.h"
#include "CRazorBeakMiniA2NState.h"
#include <Engine/CAnim3D.h>
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniA2NState::CRazorBeakMiniA2NState()
{
}

CRazorBeakMiniA2NState::~CRazorBeakMiniA2NState()
{
}


void CRazorBeakMiniA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\sru_razorbeakmini_a2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\sru_razorbeakmini_a2n", false, false, true, 0.5f);
	CJungleA2NState::Enter();
}

void CRazorBeakMiniA2NState::tick()
{
	CJungleA2NState::tick();
}

void CRazorBeakMiniA2NState::Exit()
{
	CJungleA2NState::Exit();
}

