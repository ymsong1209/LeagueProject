#pragma once


class ServerEventMgr
{
private:
    vector<tServerEvent>          m_vecEvent;


    std::chrono::steady_clock::time_point last_tick_time;
public:
    void AddEvent(const tServerEvent& _evn) {
        std::mutex m;
        m.lock();
        m_vecEvent.push_back(_evn);
        m.unlock();
    }

public:
    void sendtick(ClientServiceRef _service);
    void clienttick();


static ServerEventMgr* GetInst()
{
    static ServerEventMgr inst;
    return &inst;
}

private:
    ServerEventMgr();
    ~ServerEventMgr();
};
