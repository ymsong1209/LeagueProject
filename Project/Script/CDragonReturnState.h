#pragma once
#include "CUnitState.h"
class CDragonReturnState :
    public CUnitState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    void HandleEvent(CGameEvent& event) override;
public:
    CDragonReturnState();
    ~CDragonReturnState();
};

