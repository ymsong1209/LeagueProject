#include "pch.h"
#include "CRedDeathState.h"
#include "CRedScript.h"

CRedDeathState::CRedDeathState()
{
}

CRedDeathState::~CRedDeathState()
{
}


void CRedDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_red\\Death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_red\\Death", false, false, false, 0.f);

	Vec3 RedPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\red\\death.mp3", 1, 0.5f, true, 120.f, RedPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CRedDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRedDeathState::Exit()
{
	CJungleDeathState::Exit();
}

