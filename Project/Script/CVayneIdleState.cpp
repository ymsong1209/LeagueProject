#include "pch.h"
#include "CVayneIdleState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

CVayneIdleState::CVayneIdleState()
{
	SetName(L"Idle");
}

CVayneIdleState::~CVayneIdleState()
{
}

void CVayneIdleState::tick()
{
	// SFX / Voice Àç»ý
}

void CVayneIdleState::Enter()
{
	//GetOwner()->Animator3D()->PlayRepeat(L"Vayne\\Run_Base.001", true, 0.15f);
	//GetOwner()->Animator3D()->PlayRepeat(L"Vayne\\Idle1.001", false, true, 0.f);
	GetOwner()->Animator3D()->PlayRepeat(L"Vayne\\Idle1.001", true, true,  0.15f);
	//GetOwner()->Animator3D()->PlayLoop(L"Jinx\\Idle1_Base", true, true, 0.1f);
	CChampionIdleState::Enter();
}

void CVayneIdleState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Vayne\\Idle1.001")->Reset();
	CChampionIdleState::Exit();
}

void CVayneIdleState::HandleEvent(CGameEvent& event)
{
	CChampionIdleState::HandleEvent(event);
}

 