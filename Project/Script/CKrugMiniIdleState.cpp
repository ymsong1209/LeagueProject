#include "pch.h"
#include "CKrugMiniIdleState.h"
#include <Engine/CAnim3D.h>
#include "CKrugMiniScript.h"
CKrugMiniIdleState::CKrugMiniIdleState()
{
}

CKrugMiniIdleState::~CKrugMiniIdleState()
{
}

void CKrugMiniIdleState::Enter()
{

	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_idle_normal");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_idle_normal", false, false, false, 0.f);
}

void CKrugMiniIdleState::tick()
{
	CJungleIdleState::tick();
}

void CKrugMiniIdleState::Exit()
{

	CJungleIdleState::Exit();
}

