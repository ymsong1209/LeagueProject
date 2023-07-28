#pragma once
#include "CUnitState.h"
class CMinionWalkState :
    public CUnitState
{
public:
    CMinionWalkState();
    ~CMinionWalkState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

