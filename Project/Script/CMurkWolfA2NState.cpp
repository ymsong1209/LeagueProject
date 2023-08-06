#include "pch.h"
#include "CMurkWolfA2NState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfScript.h"
CMurkWolfA2NState::CMurkWolfA2NState()
{
}

CMurkWolfA2NState::~CMurkWolfA2NState()
{
}


void CMurkWolfA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_idle_a2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\sru_murkwolf_idle_a2n", false, false, true, 0.5f);
}

void CMurkWolfA2NState::tick()
{
	CJungleA2NState::tick();
}

void CMurkWolfA2NState::Exit()
{
	CJungleA2NState::Exit();
}

