#include "pch.h"
#include "CKrugMiniChaseState.h"
#include <Engine/CAnim3D.h>
#include "CKrugMiniScript.h"
CKrugMiniChaseState::CKrugMiniChaseState()
{
}

CKrugMiniChaseState::~CKrugMiniChaseState()
{
}


void CKrugMiniChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug_Mini\\krug_run2", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_run2", true, false, false, 0.f);
	CJungleChaseState::Enter();
}

void CKrugMiniChaseState::tick()
{
	CJungleChaseState::tick();
}

void CKrugMiniChaseState::Exit()
{
	CJungleChaseState::Exit();
}

