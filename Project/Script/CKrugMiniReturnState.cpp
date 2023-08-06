#include "pch.h"
#include "CKrugMiniReturnState.h"
#include "CKrugMiniScript.h"
CKrugMiniReturnState::CKrugMiniReturnState()
{
}

CKrugMiniReturnState::~CKrugMiniReturnState()
{
}


void CKrugMiniReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug_Mini\\krug_run2", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_run2", true, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CKrugMiniReturnState::tick()
{
	CJungleReturnState::tick();
}

void CKrugMiniReturnState::Exit()
{
	CJungleReturnState::Exit();
}

