#include "pch.h"
#include "CMurkWolfMiniChaseState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfMiniScript.h"

CMurkWolfMiniChaseState::CMurkWolfMiniChaseState()
{
}

CMurkWolfMiniChaseState::~CMurkWolfMiniChaseState()
{
}


void CMurkWolfMiniChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf_Mini\\sru_murkwolf_pounce", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf_Mini\\sru_murkwolf_pounce", false, false, false, 0.f);
	CJungleChaseState::Enter();
}

void CMurkWolfMiniChaseState::tick()
{
	CJungleChaseState::tick();
}

void CMurkWolfMiniChaseState::Exit()
{
	CJungleChaseState::Exit();
}
