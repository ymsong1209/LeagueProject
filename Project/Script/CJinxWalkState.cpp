#include "pch.h"
#include "CJinxWalkState.h"
#include <Engine/CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"


CJinxWalkState::CJinxWalkState()
{
	SetName(L"Run");
}

CJinxWalkState::~CJinxWalkState()
{
}

void CJinxWalkState::tick()
{
	// SFX / Voice Àç»ý
}

void CJinxWalkState::Enter()
{
	wstring animName = L"Jinx\\Run_Base";
	GetOwner()->Animator3D()->PlayRepeat(L"Jinx\\Run_Base", true, true, 0.15f);

	SendAnimPacket(animName, true, true, 0.15f);

	CChampionWalkState::Enter();
}

void CJinxWalkState::Exit()
{
	//GetOwner()->Animator3D()->FindAnim(L"Jinx\\Run_Base")->Reset();
	CChampionWalkState::Exit();
}

void CJinxWalkState::HandleEvent(CGameEvent& event)
{
	CChampionWalkState::HandleEvent(event);
}
