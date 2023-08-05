#pragma once
#include "CJungleN2AState.h"
class CBlueN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CBlueN2AState();
    ~CBlueN2AState();
};

