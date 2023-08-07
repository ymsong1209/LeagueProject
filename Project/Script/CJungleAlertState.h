#pragma once
#include "CUnitState.h"
class CJungleAlertState :
    public CUnitState
{
private:
    bool        m_bDetectChampion;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    //void HandleEvent(CGameEvent& event) override;


public:
    CJungleAlertState();
    ~CJungleAlertState();
};

