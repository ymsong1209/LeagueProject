#include "pch.h"
#include "CRazorBeakDeathState.h"
#include "CRazorBeakScript.h"

CRazorBeakDeathState::CRazorBeakDeathState()
{
}

CRazorBeakDeathState::~CRazorBeakDeathState()
{
}


void CRazorBeakDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"RazorBeak\\Death_Base");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"RazorBeak\\Death_Base", false, false, false, 0.f);

	Vec3 RazorbeakPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\razorbeak\\death.mp3", 1, 0.3f, true, 120.f, RazorbeakPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CRazorBeakDeathState::tick()
{
	CJungleDeathState::tick();
}


void CRazorBeakDeathState::Exit()
{
	CJungleDeathState::Exit();
}

