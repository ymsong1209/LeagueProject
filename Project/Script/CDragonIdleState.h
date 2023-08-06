#pragma once
#include "CUnitState.h"
class CDragonIdleState :
    public CUnitState
{
public:
    virtual void Enter();
    virtual void tick();
    virtual void Exit();
public:
    void HandleEvent(CGameEvent& event) override;
public:
    CDragonIdleState();
    ~CDragonIdleState();
};

