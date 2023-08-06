#include "pch.h"
#include "CRedReturnState.h"
#include "CRedScript.h"

CRedReturnState::CRedReturnState()
{
}

CRedReturnState::~CRedReturnState()
{
}


void CRedReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_red\\Run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_red\\Run", false, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CRedReturnState::tick()
{
	CJungleReturnState::tick();
}

void CRedReturnState::Exit()
{
	CJungleReturnState::Exit();
}

