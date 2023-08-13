#pragma once
#include "CUnitState.h"
class CInhibitorIdleState :
    public CUnitState
{
public:
    CInhibitorIdleState();
    ~CInhibitorIdleState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual void HandleEvent(CGameEvent& event);
};

