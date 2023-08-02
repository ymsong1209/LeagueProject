#pragma once
#include "CUnitState.h"
class CJungleReturnState :
    public CUnitState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CJungleReturnState();
    ~CJungleReturnState();
};

