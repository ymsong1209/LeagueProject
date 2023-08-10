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




