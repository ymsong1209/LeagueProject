#include "pch.h"
#include "CKrugMiniN2AState.h"
#include <Engine/CAnim3D.h>
#include "CKrugMiniScript.h"
CKrugMiniN2AState::CKrugMiniN2AState()
{
}

CKrugMiniN2AState::~CKrugMiniN2AState()
{
}


void CKrugMiniN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_mini_idle_n2ag", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_mini_idle_n2ag", false, false, true, 0.5f);
}

void CKrugMiniN2AState::tick()
{
	CJungleN2AState::tick();
}

void CKrugMiniN2AState::Exit()
{
	CJungleN2AState::Exit();
}

