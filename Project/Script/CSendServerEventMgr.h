#pragma once
#include <Engine/CSingleton.h>

class CGameEvent;

//// Event
//struct tServerEvent
//{
//    SERVER_EVENT_TYPE Type;
//    DWORD_PTR   wParam;
//    DWORD_PTR   lParam;
//};

class CSendServerEventMgr :
    public CSingleton<CSendServerEventMgr>
{
    SINGLE(CSendServerEventMgr);

public:
    vector<tServerEvent> m_vecServerSendEvent;

public:
    void AddServerSendEvent(tServerEvent _evn) { m_vecServerSendEvent.push_back(_evn); }

    vector<tServerEvent>& GetVecEvent() { return m_vecServerSendEvent; }
    void ClearServerSendEvent() { m_vecServerSendEvent.clear(); }


};

