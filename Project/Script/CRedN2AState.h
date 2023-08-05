#pragma once
#include "CJungleN2AState.h"
class CRedN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRedN2AState();
    ~CRedN2AState();
};

