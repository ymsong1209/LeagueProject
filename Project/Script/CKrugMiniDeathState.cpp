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
	CJungleDeathState::Enter();
}

void CKrugMiniDeathState::tick()
{
	CJungleDeathState::tick();
}

void CKrugMiniDeathState::Exit()
{
}
