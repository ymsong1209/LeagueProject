#include "pch.h"
#include "CBlueN2AState.h"
#include <Engine/CAnim3D.h>
#include "CBlueScript.h"
CBlueN2AState::CBlueN2AState()
{
}

CBlueN2AState::~CBlueN2AState()
{
}


void CBlueN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_blue\\sru_blue_idle_n2a", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\sru_blue_idle_n2a", false, false, false, 0.5f);
}

void CBlueN2AState::tick()
{
	CJungleN2AState::tick();
}

void CBlueN2AState::Exit()
{
	CJungleN2AState::Exit();
}

