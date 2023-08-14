#pragma once
#include "CUnitState.h"
class CNexusIdleState :
    public CUnitState
{
public:
    CNexusIdleState();
    ~CNexusIdleState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual void HandleEvent(CGameEvent& event);
};

