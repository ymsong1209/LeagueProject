#include "pch.h"
#include "CRazorBeakMiniDeathState.h"
#include "CRazorBeakMiniScript.h"

CRazorBeakMiniDeathState::CRazorBeakMiniDeathState()
{
}

CRazorBeakMiniDeathState::~CRazorBeakMiniDeathState()
{
}


void CRazorBeakMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak_Mini\\Death_Base");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak_Mini\\Death_Base", false, false, false, 0.f);

	Vec3 RazorbeakPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\razorbeak_mini\\death.mp3", 1, 0.3f, true, 120.f, RazorbeakPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CRazorBeakMiniDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRazorBeakMiniDeathState::Exit()
{
	CJungleDeathState::Exit();
}

