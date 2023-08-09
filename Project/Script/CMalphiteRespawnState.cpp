#include "pch.h"
#include "CMalphiteRespawnState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CUnitScript.h"

CMalphiteRespawnState::CMalphiteRespawnState()
{
	SetName(L"Respawn");
}

CMalphiteRespawnState::~CMalphiteRespawnState()
{
}

void CMalphiteRespawnState::tick()
{
}

void CMalphiteRespawnState::Enter()
{
	CChampionRespawnState::Enter();
	GetOwner()->Fsm()->ChangeState(L"Idle");
}

void CMalphiteRespawnState::Exit()
{
	CChampionRespawnState::Exit();
}

void CMalphiteRespawnState::HandleEvent(CGameEvent& event)
{
	CChampionRespawnState::HandleEvent(event);
}
