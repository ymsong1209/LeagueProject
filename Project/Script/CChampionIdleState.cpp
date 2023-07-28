#include "pch.h"
#include "CChampionIdleState.h"
#include <Engine/CFsm.h>

#include "CChampionAttackState.h"
#include "CGameEvent.h"

CChampionIdleState::CChampionIdleState()
{
}

CChampionIdleState::~CChampionIdleState()
{
}

void CChampionIdleState::tick()
{
}

void CChampionIdleState::Enter()
{
	CUnitState::Enter();
}

void CChampionIdleState::Exit()
{
	CUnitState::Exit();
}

void CChampionIdleState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;
		
	case GAME_EVENT_TYPE::PLAYER_BASE_ATTACK:
	{
		BaseAttackEvent* AttackEvent = dynamic_cast<BaseAttackEvent*>(&event);

		CChampionAttackState* AttackState =  dynamic_cast<CChampionAttackState*>(GetOwnerFSM()->FindState(L"Attack"));
		if (AttackState != nullptr)
		{
			AttackState->SetUserID(AttackEvent->GetUserID());
			AttackState->SetTargetID(AttackEvent->GetTargetID());
		}
		GetOwnerFSM()->ChangeState(L"Attack");
	}
		break;

	case GAME_EVENT_TYPE::PLAYER_MOVE:
		GetOwnerFSM()->ChangeState(L"Walk");
		break;
	case GAME_EVENT_TYPE::PLAYER_SKILL_Q:
	{
		if(GetOwnerFSM()->FindState(L"Q") != nullptr)
			GetOwnerFSM()->ChangeState(L"Q");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_W:
	{
		if (GetOwnerFSM()->FindState(L"W") != nullptr)
			GetOwnerFSM()->ChangeState(L"W");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_E:
	{
		if (GetOwnerFSM()->FindState(L"E") != nullptr)
			GetOwnerFSM()->ChangeState(L"E");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_R:
	{
		if (GetOwnerFSM()->FindState(L"R") != nullptr)
			GetOwnerFSM()->ChangeState(L"R");
		break;
	}
	}
}
