#include "pch.h"
#include "CMurkWolfDeathState.h"
#include "CMurkWolfScript.h"
CMurkWolfDeathState::CMurkWolfDeathState()
{
}

CMurkWolfDeathState::~CMurkWolfDeathState()
{
}


void CMurkWolfDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\sru_murkwolf_death", false, false, false, 0.f);

	Vec3 MurkWolfPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\murkwolf\\death.mp3", 1, 0.5f, true, 200.f, MurkWolfPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CMurkWolfDeathState::tick()
{
	CJungleDeathState::tick();
}


void CMurkWolfDeathState::Exit()
{
	CJungleDeathState::Exit();
}



