#include "pch.h"
#include "CKrugMiniA2NState.h"
#include <Engine/CAnim3D.h>
#include "CKrugMiniScript.h"

CKrugMiniA2NState::CKrugMiniA2NState()
{
}

CKrugMiniA2NState::~CKrugMiniA2NState()
{
}


void CKrugMiniA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug_Mini\\krug_idle_ag2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Krug_Mini\\krug_idle_ag2n", false, false, true, 0.5f);

}

void CKrugMiniA2NState::tick()
{
	CJungleA2NState::tick();
}

void CKrugMiniA2NState::Exit()
{
	CJungleA2NState::Exit();
}
