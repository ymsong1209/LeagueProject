#include "pch.h"
#include "CKrugDeathState.h"
#include "CKrugScript.h"
CKrugDeathState::CKrugDeathState()
{
}

CKrugDeathState::~CKrugDeathState()
{
}


void CKrugDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_death");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_death", false, false, false, 0.f);

	Vec3 KrugPos = GetOwner()->Transform()->GetRelativePos();
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\krug\\death.mp3", 1, 0.5f, true, 200.f, KrugPos, Faction::NONE);
	CJungleDeathState::Enter();
}

void CKrugDeathState::tick()
{
	CJungleDeathState::tick();
}

void CKrugDeathState::Exit()
{
	CJungleDeathState::Exit();
}




