#pragma once
#include "CUnitState.h"
class CNexusBrokenState :
    public CUnitState
{
public:
    CNexusBrokenState();
    ~CNexusBrokenState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

