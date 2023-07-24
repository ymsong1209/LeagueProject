#include "pch.h"
#include "CChampionSkillState.h"
#include <Engine/CTimeMgr.h>
#include <Engine/CFsm.h>

CChampionSkillState::CChampionSkillState()
{
}

CChampionSkillState::~CChampionSkillState()
{
}

void CChampionSkillState::tick()
{
	// 캐스팅 시간
	if (m_fCurCastingTime > 0.f)
		m_fCurCastingTime -= DT;
	else
		m_fCurCastingTime = 0.f;

	
}

void CChampionSkillState::Enter()
{
	CUnitState::Enter();
}

void CChampionSkillState::Exit()
{
	CUnitState::Exit();
	m_fCurCastingTime = m_fCastingTime;
}

void CChampionSkillState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;

	case GAME_EVENT_TYPE::PLAYER_ATTACK:
	{
		if(!m_fCurCastingTime > 0.0f)
			GetOwnerFSM()->ChangeState(L"Attack");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_MOVE:
	{
		if (!m_fCurCastingTime > 0.0f)
			GetOwnerFSM()->ChangeState(L"Walk");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_Q:
	{
		if (!m_fCurCastingTime > 0.0f)
		{
			if (GetOwnerFSM()->FindState(L"Q") != nullptr)
					GetOwnerFSM()->ChangeState(L"Q");
		}
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_W:
	{
		if (!m_fCurCastingTime > 0.0f)
		{
			if (GetOwnerFSM()->FindState(L"W") != nullptr)
				GetOwnerFSM()->ChangeState(L"W");
		}
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_E:
	{
		if (!m_fCurCastingTime > 0.0f)
		{
			if (GetOwnerFSM()->FindState(L"E") != nullptr)
				GetOwnerFSM()->ChangeState(L"E");
			break;
		}
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_R:
	{
		if (!m_fCurCastingTime > 0.0f)
		{
			if (GetOwnerFSM()->FindState(L"R") != nullptr)
				GetOwnerFSM()->ChangeState(L"R");
			break;
		}
	}
	}
}
