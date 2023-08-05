#pragma once
#include "CJungleIdleState.h"
class CBlueIdleState :
    public CJungleIdleState
{
private:
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CBlueIdleState();
    ~CBlueIdleState();
};

