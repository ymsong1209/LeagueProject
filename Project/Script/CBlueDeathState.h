#pragma once
#include "CJungleDeathState.h"
class CBlueDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CBlueDeathState();
    ~CBlueDeathState();
};

