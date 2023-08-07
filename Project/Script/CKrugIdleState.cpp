#include "pch.h"
#include "CKrugIdleState.h"
#include <Engine/CAnim3D.h>
#include "CKrugScript.h"
CKrugIdleState::CKrugIdleState()
{
}

CKrugIdleState::~CKrugIdleState()
{
}

void CKrugIdleState::Enter()
{
	CJungleIdleState::Enter();
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_idle_normal");
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_idle_normal", false, false, false, 0.f);
}

void CKrugIdleState::tick()
{
	CJungleIdleState::tick();
}

void CKrugIdleState::Exit()
{

	CJungleIdleState::Exit();
}

