#pragma once
#include "CJungleChaseState.h"
class CBlueChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CBlueChaseState();
    ~CBlueChaseState();
};

