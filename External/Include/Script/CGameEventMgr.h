#pragma once
#include <Engine/CSingleton.h>

class IGameEventListener;
class CGameEvent;

class CGameEventMgr :
	public CSingleton<CGameEventMgr>
{
	SINGLE(CGameEventMgr);

private:
	//CGameEvent* m_vecEvent[(UINT)GAME_EVENT_TYPE::END];
	
	vector<CGameEvent*> m_vecGameEventList;
	
	vector<IGameEventListener*>	m_vecListener;

public:
	void AddListener(IGameEventListener* _listener) { m_vecListener.push_back(_listener); }
	//CGameEvent* GetEvent(UINT _type) { return m_vecEvent[(UINT)_type]; }
	vector<CGameEvent*>& GetEventList() { return m_vecGameEventList; }


	void NotifyEvent(CGameEvent& _event);

};

