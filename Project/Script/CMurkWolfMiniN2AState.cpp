#include "pch.h"
#include "CMurkWolfMiniN2AState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniN2AState::CMurkWolfMiniN2AState()
{
}

CMurkWolfMiniN2AState::~CMurkWolfMiniN2AState()
{
}


void CMurkWolfMiniN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf_Mini\\sru_murkwolfmini_idle1_n2a", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf_Mini\\sru_murkwolfmini_idle1_n2a", false, false, true, 0.5f);
	CJungleN2AState::Enter();
}

void CMurkWolfMiniN2AState::tick()
{
	CJungleN2AState::tick();
}

void CMurkWolfMiniN2AState::Exit()
{
	CJungleN2AState::Exit();
}
