#pragma once
#include "CUnitState.h"
class CJungleChaseState :
    public CUnitState
{

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CJungleChaseState();
    ~CJungleChaseState();
};

