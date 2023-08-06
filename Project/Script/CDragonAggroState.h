#pragma once
#include "CUnitState.h"
class CDragonAggroState :
    public CUnitState
{
private:
    int         m_iAggroAnimNum;
    bool        m_bDetectChampion;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    void HandleEvent(CGameEvent& event) override;


public:
    CDragonAggroState();
    ~CDragonAggroState();
};

