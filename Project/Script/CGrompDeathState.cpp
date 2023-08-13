#include "pch.h"
#include "CGrompDeathState.h"
#include "CGrompScript.h"
CGrompDeathState::CGrompDeathState()
{
}

CGrompDeathState::~CGrompDeathState()
{
}


void CGrompDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_death", false,false, false, 0.0f);
	CJungleDeathState::Enter();
}

void CGrompDeathState::tick()
{
	CJungleDeathState::tick();
}

void CGrompDeathState::Exit()
{
	CJungleDeathState::Exit();
}




