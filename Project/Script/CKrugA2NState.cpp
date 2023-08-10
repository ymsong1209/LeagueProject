#include "pch.h"
#include "CKrugA2NState.h"
#include <Engine/CAnim3D.h>
#include "CKrugScript.h"
CKrugA2NState::CKrugA2NState()
{
}

CKrugA2NState::~CKrugA2NState()
{
}


void CKrugA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_idle_ag2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug\\krug_idle_ag2n", false, false, true, 0.5f);
	CJungleA2NState::Enter();
}

void CKrugA2NState::tick()
{
	CJungleA2NState::tick();
}

void CKrugA2NState::Exit()
{
	CJungleA2NState::Exit();
}

