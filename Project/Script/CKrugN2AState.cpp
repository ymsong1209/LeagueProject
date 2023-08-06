#include "pch.h"
#include "CKrugN2AState.h"
#include <Engine/CAnim3D.h>
#include "CKrugScript.h"
CKrugN2AState::CKrugN2AState()
{
}

CKrugN2AState::~CKrugN2AState()
{
}


void CKrugN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_idle_n2ag", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_idle_n2ag", false, false, true, 0.5f);
}

void CKrugN2AState::tick()
{
	CJungleN2AState::tick();
}

void CKrugN2AState::Exit()
{
	CJungleN2AState::Exit();
}

