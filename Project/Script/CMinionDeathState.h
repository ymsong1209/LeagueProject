#pragma once
#include "CUnitState.h"
class CMinionDeathState :
    public CUnitState
{
public:
    CMinionDeathState();
    ~CMinionDeathState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

