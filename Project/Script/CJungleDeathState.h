#pragma once
#include "CUnitState.h"
class CJungleDeathState :
    public CUnitState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    CJungleDeathState();
    ~CJungleDeathState();
};

