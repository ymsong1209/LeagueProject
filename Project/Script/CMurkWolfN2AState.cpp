#include "pch.h"
#include "CMurkWolfN2AState.h"
#include <Engine/CAnim3D.h>
#include "CMurkWolfScript.h"
CMurkWolfN2AState::CMurkWolfN2AState()
{
}

CMurkWolfN2AState::~CMurkWolfN2AState()
{
}


void CMurkWolfN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"MurkWolf\\sru_murkwolf_idle_n2a", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"MurkWolf\\sru_murkwolf_idle_n2a", false, false, true, 0.5f);

	Vec3 MurkWolfPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\murkwolf\\n2a.mp3", 1, 0.5f, true, 200.f, MurkWolfPos, Faction::NONE);

	CJungleN2AState::Enter();
}

void CMurkWolfN2AState::tick()
{
	CJungleN2AState::tick();
}

void CMurkWolfN2AState::Exit()
{
	CJungleN2AState::Exit();
}

