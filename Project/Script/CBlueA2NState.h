#pragma once
#include "CJungleA2NState.h"
class CBlueA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CBlueA2NState();
    ~CBlueA2NState();
};

