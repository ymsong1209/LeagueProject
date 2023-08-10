#pragma once
#include "CUnitState.h"
class CTurretIdleState :
    public CUnitState
{
public:
    CTurretIdleState();
    ~CTurretIdleState();

public:
    virtual void Enter() override;
    virtual void Exit() override;

public:
    virtual void HandleEvent(CGameEvent& event);
};

