#pragma once
#include "CUnitState.h"
class CDragonAggro2AlertState :
    public CUnitState
{
private:
   
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    void HandleEvent(CGameEvent& event) override;


public:
    CDragonAggro2AlertState();
    ~CDragonAggro2AlertState();
};

