#pragma once
#include "CUnitState.h"
class CMinionChaseState :
    public CUnitState
{
public:
    CMinionChaseState();
    ~CMinionChaseState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

