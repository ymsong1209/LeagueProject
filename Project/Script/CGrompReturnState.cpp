#include "pch.h"
#include "CGrompReturnState.h"
#include "CGrompScript.h"
CGrompReturnState::CGrompReturnState()
{
}

CGrompReturnState::~CGrompReturnState()
{
}


void CGrompReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"gromp\\_run",false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_run", true, true, 0.0f);
	CJungleReturnState::Enter();
}

void CGrompReturnState::tick()
{
	CJungleReturnState::tick();
}

void CGrompReturnState::Exit()
{
	CJungleReturnState::Exit();
}

