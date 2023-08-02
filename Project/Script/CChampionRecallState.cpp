#include "pch.h"
#include "CChampionRecallState.h"
#include <Engine/CFsm.h>
#include <Engine\CTimeMgr.h>

#include "CChampionAttackState.h"
#include "CGameEvent.h"

CChampionRecallState::CChampionRecallState()
	: m_fRecallTime(8.0f)
{
}

CChampionRecallState::~CChampionRecallState()
{
}

void CChampionRecallState::tick()
{
	m_fRecallTime -= DT;
	if (m_fRecallTime <= 0)
	{
		// 우물로 Pos 변경
		GetOwnerFSM()->ChangeState(L"Respawn");
	}
}

void CChampionRecallState::Enter()
{
	CUnitState::Enter();
}

void CChampionRecallState::Exit()
{
	// 귀환 시간 초기화
	m_fRecallTime = 8.f;

	CUnitState::Exit();
}

void CChampionRecallState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;

	case GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK:
	{
		BasicAttackEvent* AttackEvent = dynamic_cast<BasicAttackEvent*>(&event);

		CChampionAttackState* AttackState = dynamic_cast<CChampionAttackState*>(GetOwnerFSM()->FindState(L"Attack"));
		if (AttackState != nullptr)
		{
			AttackState->SetUserObj(AttackEvent->GetUserObj());
			AttackState->SetTargetObj(AttackEvent->GetTargetObj());
		}
		GetOwnerFSM()->ChangeState(L"Attack");
	}
	break;

	case GAME_EVENT_TYPE::PLAYER_MOVE:
		GetOwnerFSM()->ChangeState(L"Walk");
		break;
	case GAME_EVENT_TYPE::PLAYER_SKILL_Q:
	{
		if (GetOwnerFSM()->FindState(L"Q") != nullptr)
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
