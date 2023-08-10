#include "pch.h"
#include "CMalphiteWalkState.h"
#include <Engine/CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"


CMalphiteWalkState::CMalphiteWalkState()
{
	SetName(L"Run");
}

CMalphiteWalkState::~CMalphiteWalkState()
{
}

void CMalphiteWalkState::Enter()
{
	wstring animName = L"Malphite\\Run";
	GetOwner()->Animator3D()->PlayRepeat(L"Malphite\\Run", false , true, 0.5f);


	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, true, false, true, 0.15f);

	CChampionWalkState::Enter();
}


void CMalphiteWalkState::tick()
{
	// SFX / Voice Àç»ý
}


void CMalphiteWalkState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CChampionWalkState::Exit();
}

void CMalphiteWalkState::HandleEvent(CGameEvent& event)
{
	CChampionWalkState::HandleEvent(event);
}
