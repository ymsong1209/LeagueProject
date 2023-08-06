#include "pch.h"
#include "CBlueReturnState.h"
#include "CBlueScript.h"
CBlueReturnState::CBlueReturnState()
{
}

CBlueReturnState::~CBlueReturnState()
{
}


void CBlueReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_blue\\Run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\Run", true, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CBlueReturnState::tick()
{
	CJungleReturnState::tick();
}

void CBlueReturnState::Exit()
{
	CJungleReturnState::Exit();
}

