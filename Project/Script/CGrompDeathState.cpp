#include "pch.h"
#include "CGrompDeathState.h"
#include "CGrompScript.h"
CGrompDeathState::CGrompDeathState()
{
}

CGrompDeathState::~CGrompDeathState()
{
}


void CGrompDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_death", false,false, false, 0.0f);

	Vec3 GrompPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\gromp\\death.mp3", 1, 0.5f, true, 200.f, GrompPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CGrompDeathState::tick()
{
	CJungleDeathState::tick();
}

void CGrompDeathState::Exit()
{
	CJungleDeathState::Exit();
}




