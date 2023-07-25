#pragma once
#include "CSingleton.h"



class CEventMgr :
    public CSingleton< CEventMgr>
{
    SINGLE(CEventMgr);
private:
    vector<tEvent>          m_vecEvent;    
    vector<CGameObject*>    m_vecGC;

    bool                    m_LevelChanged;

public:
    void AddEvent(const tEvent& _evn) { m_vecEvent.push_back(_evn); }
    bool IsLevelChanged() { return m_LevelChanged; }

    //InspectorUI가 GC를 알아야지 object가 삭제예정인지 확인 가능
    vector<CGameObject*>    GetGC() {return m_vecGC;}
private:
    void GC_Clear();

public:
    void tick();
};

