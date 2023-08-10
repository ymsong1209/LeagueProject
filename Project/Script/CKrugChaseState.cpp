#include "pch.h"
#include "CKrugChaseState.h"
#include <Engine/CAnim3D.h>
#include "CKrugScript.h"
CKrugChaseState::CKrugChaseState()
{
}

CKrugChaseState::~CKrugChaseState()
{
}


void CKrugChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug\\krug_run2", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_run2", true, false, false, 0.f);
	CJungleChaseState::Enter();
}

void CKrugChaseState::tick()
{
	CJungleChaseState::tick();
}

void CKrugChaseState::Exit()
{
	CJungleChaseState::Exit();
}

