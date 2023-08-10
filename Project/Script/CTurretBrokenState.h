#pragma once
#include "CUnitState.h"
class CTurretBrokenState :
    public CUnitState
{
public:
    CTurretBrokenState();
    ~CTurretBrokenState();

public:
    virtual void Enter() override;
    virtual void Exit() override;
};

