#include "pch.h"
#include "CBlueDeathState.h"
#include "CBlueScript.h"
CBlueDeathState::CBlueDeathState()
{
}

CBlueDeathState::~CBlueDeathState()
{
}


void CBlueDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"jungle_blue\\Death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_blue\\Death", false, false, false, 0.f);

	Vec3 BluePos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\blue\\death.mp3", 1, 0.5f, true, 130.f, BluePos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CBlueDeathState::tick()
{
	CJungleDeathState::tick();
}

void CBlueDeathState::Exit()
{
	CJungleDeathState::Exit();
}



