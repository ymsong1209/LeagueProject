#pragma once
#include "CUnitState.h"
class CDragonAlertState :
    public CUnitState
{
private:
    int         m_iAlertAnimNum;
    bool        m_bDetectChampion;
    bool        m_bChampInAggro;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    void HandleEvent(CGameEvent& event) override;


public:
    CDragonAlertState();
    ~CDragonAlertState();
};

