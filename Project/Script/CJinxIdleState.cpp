#include "pch.h"
#include "CJinxIdleState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"
CJinxIdleState::CJinxIdleState()
{
	SetName(L"Idle");
}

CJinxIdleState::~CJinxIdleState()
{
}

void CJinxIdleState::tick()
{
	// SFX / Voice Àç»ý
}

void CJinxIdleState::Enter()
{
	wstring animName = L"Jinx\\Idle1_Base";
	GetOwner()->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true, true, 0.15f);

	SendAnimPacket(animName, true, true, 0.15f);
	
	CChampionIdleState::Enter();
}

void CJinxIdleState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Jinx\\Idle1_Base")->Reset();
	CChampionIdleState::Exit();
}

void CJinxIdleState::HandleEvent(CGameEvent& event)
{
	CChampionIdleState::HandleEvent(event);
}
