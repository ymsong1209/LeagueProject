#include "pch.h"
#include "CVayneDeathState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

CVayneDeathState::CVayneDeathState()
{
	SetName(L"Death");
}

CVayneDeathState::~CVayneDeathState()
{
}

void CVayneDeathState::tick()
{
}

void CVayneDeathState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Vayne\\Death.001", false, 0.1f);

	CChampionDeathState::Enter();
}

void CVayneDeathState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Vayne\\Death.001")->Reset();
	CChampionDeathState::Exit();
}

void CVayneDeathState::HandleEvent(CGameEvent& event)
{
	CChampionDeathState::HandleEvent(event);
}

 