#include "pch.h"
#include "CGameEventMgr.h"
#include "IGameEventListener.h"
#include "CGameEvent.h"

CGameEventMgr::CGameEventMgr()
{
	m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_DEATH] = new DeathEvent;
	m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_RESPAWN] = new RespawnEvent;
	m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_STOP] = new StopEvent;
	m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_MOVE] = new MoveEvent;
	//m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_KILL_CHAMPION] = new CGameEvent;
	//m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_KILL_MOB] = new CGameEvent;
	//m_vecEvent[(UINT)GAME_EVENT_TYPE::PLAYER_ATTACK] = new CGameEvent;
}

CGameEventMgr::~CGameEventMgr()
{
	Safe_Del_Vec(m_vecListener);

	Safe_Del_Array(m_vecEvent);
}

void CGameEventMgr::NotifyEvent(CGameEvent& _event)
{
	for (auto listener : m_vecListener)
	{
		listener->onEvent(_event);
	}
}
