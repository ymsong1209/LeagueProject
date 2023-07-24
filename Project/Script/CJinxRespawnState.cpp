#include "pch.h"
#include "CJinxRespawnState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

CJinxRespawnState::CJinxRespawnState()
{
	SetName(L"Respawn");
}

CJinxRespawnState::~CJinxRespawnState()
{
}

void CJinxRespawnState::tick()
{
}

void CJinxRespawnState::Enter()
{
	GetOwner()->Animator3D()->Play(L"Jinx\\Respawn", true, 0.1f);
	CChampionRespawnState::Enter();
}

void CJinxRespawnState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Jinx\\Respawn")->Reset();
	CChampionRespawnState::Exit();
}

void CJinxRespawnState::HandleEvent(CGameEvent& event)
{
	CChampionRespawnState::HandleEvent(event);
}
