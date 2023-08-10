#include "pch.h"
#include "CMurkWolfChaseState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfScript.h"
CMurkWolfChaseState::CMurkWolfChaseState()
{
}

CMurkWolfChaseState::~CMurkWolfChaseState()
{
}


void CMurkWolfChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"MurkWolf\\sru_murkwolf_pounce", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\sru_murkwolf_pounce", true, false, false, 0.f);
	CJungleChaseState::Enter();
}

void CMurkWolfChaseState::tick()
{
	CJungleChaseState::tick();
}

void CMurkWolfChaseState::Exit()
{
	CJungleChaseState::Exit();
}

