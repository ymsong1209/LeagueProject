#include "pch.h"
#include "CMurkWolfMiniA2NState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniA2NState::CMurkWolfMiniA2NState()
{
}

CMurkWolfMiniA2NState::~CMurkWolfMiniA2NState()
{
}


void CMurkWolfMiniA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf_Mini\\sru_murkwolfmini_idle1_a2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf_Mini\\sru_murkwolfmini_idle1_a2n", false, false, true, 0.5f);
}

void CMurkWolfMiniA2NState::tick()
{
	CJungleA2NState::tick();
}

void CMurkWolfMiniA2NState::Exit()
{
	CJungleA2NState::Exit();
}

