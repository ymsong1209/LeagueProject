#include "pch.h"
#include "CChampionDeathState.h"
#include <Engine/CFsm.h>

CChampionDeathState::CChampionDeathState()
{
}

CChampionDeathState::~CChampionDeathState()
{
}

void CChampionDeathState::tick()
{
}

void CChampionDeathState::Enter()
{
	CUnitState::Enter();
}

void CChampionDeathState::Exit()
{
	CUnitState::Exit();
}

void CChampionDeathState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_RESPAWN:
	{
		// 우물로 이동
		GetOwnerFSM()->ChangeState(L"Respawn");
		break;
	}
	}
}
