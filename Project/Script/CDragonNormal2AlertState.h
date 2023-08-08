#pragma once
#include "CUnitState.h"
class CDragonNormal2AlertState :
    public CUnitState
{

private:
 
public:
    virtual void Enter();
    virtual void tick();
    virtual void Exit();
public:
    void HandleEvent(CGameEvent& event) override;
public:
    CDragonNormal2AlertState();
    ~CDragonNormal2AlertState();
};
