#include "pch.h"
#include "CVayneRespawnState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

CVayneRespawnState::CVayneRespawnState()
{
	SetName(L"Respawn");
}

CVayneRespawnState::~CVayneRespawnState()
{
}

void CVayneRespawnState::tick()
{
	CChampionRespawnState::tick();
}

void CVayneRespawnState::Enter()
{
	//GetOwner()->Animator3D()->Play(L"Jinx\\Respawn", true, 0.1f);
	GetOwner()->Animator3D()->PlayOnce(L"Vayne\\Respawn.001");
	CChampionRespawnState::Enter();
}

void CVayneRespawnState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Vayne\\Respawn.001")->Reset();
	CChampionRespawnState::Exit();
}

void CVayneRespawnState::HandleEvent(CGameEvent& event)
{
	CChampionRespawnState::HandleEvent(event);
}

 
