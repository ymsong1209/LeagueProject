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
    bool                    m_bIsGameOver;

public:
    void AddEvent(const tEvent& _evn) { m_vecEvent.push_back(_evn); }
    bool IsLevelChanged() { return m_LevelChanged; }
    void SetIsLevelChanged(bool _IsResetOuliner) { m_LevelChanged = _IsResetOuliner; } //�ƿ����̳� ���ΰ�ħ

    //InspectorUI�� GC�� �˾ƾ��� object�� ������������ Ȯ�� ����
    vector<CGameObject*>    GetGC() {return m_vecGC;}

    bool GetIsGameOver() { return m_bIsGameOver; }
    void SetIsGameOver(bool _IsOver) { m_bIsGameOver = _IsOver; }

private:
    void GC_Clear();

public:
    void tick();
};

