#include "pch.h"
#include "CRedA2NState.h"
#include <Engine/CAnim3D.h>
#include "CRedScript.h"

CRedA2NState::CRedA2NState()
{
}

CRedA2NState::~CRedA2NState()
{
}


void CRedA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\sru_red_idle_a2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_red\\sru_red_idle_a2n", false, false, true, 0.5f);
	CJungleA2NState::Enter();
}

void CRedA2NState::tick()
{
	CJungleA2NState::tick();
}

void CRedA2NState::Exit()
{
	CJungleA2NState::Exit();
}

