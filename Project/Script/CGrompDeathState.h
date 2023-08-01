#pragma once
#include "CJungleDeathState.h"
class CGrompDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CGrompDeathState();
    ~CGrompDeathState();
};

