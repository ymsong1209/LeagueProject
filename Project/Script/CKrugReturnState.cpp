#include "pch.h"
#include "CKrugReturnState.h"
#include "CKrugScript.h"
CKrugReturnState::CKrugReturnState()
{
}

CKrugReturnState::~CKrugReturnState()
{
}


void CKrugReturnState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Krug\\krug_run2", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_run2", true, false, false, 0.f);
	CJungleReturnState::Enter();
}

void CKrugReturnState::tick()
{
	CJungleReturnState::tick();
}

void CKrugReturnState::Exit()
{
	CJungleReturnState::Exit();
}

