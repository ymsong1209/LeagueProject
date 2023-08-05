#pragma once
#include "CJungleDeathState.h"
class CRedDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRedDeathState();
    ~CRedDeathState();
};

