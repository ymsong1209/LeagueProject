#include "pch.h"
#include "CKrugMiniDeathState.h"
#include "CKrugMiniScript.h"
CKrugMiniDeathState::CKrugMiniDeathState()
{
}

CKrugMiniDeathState::~CKrugMiniDeathState()
{
}


void CKrugMiniDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_death", false, false, false, 0.f);

	Vec3 KrugPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\krug\\death.mp3", 1, 0.3f, true, 120.f, KrugPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CKrugMiniDeathState::tick()
{
	CJungleDeathState::tick();
}

void CKrugMiniDeathState::Exit()
{
	CJungleDeathState::Exit();
}
