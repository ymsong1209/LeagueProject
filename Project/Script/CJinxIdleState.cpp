#include "pch.h"
#include "CJinxIdleState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

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
	GetOwner()->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true, true,  0.1f);
	//GetOwner()->Animator3D()->PlayLoop(L"Jinx\\Idle1_Base", true, true, 0.1f);
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
