#include "pch.h"
#include "CVayneWalkState.h"
#include <Engine/CAnimator3D.h>
#include <Engine\CAnim3D.h>

CVayneWalkState::CVayneWalkState()
{
	SetName(L"Run");
}

CVayneWalkState::~CVayneWalkState()
{
}

void CVayneWalkState::tick()
{
	// SFX / Voice Àç»ý
}

void CVayneWalkState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"Vayne\\Run_Base.001", true, 0.15f);
	CChampionWalkState::Enter();
}

void CVayneWalkState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Vayne\\Run_Base.001")->Reset();
	CChampionWalkState::Exit();
}

void CVayneWalkState::HandleEvent(CGameEvent& event)
{
	CChampionWalkState::HandleEvent(event);
}

 