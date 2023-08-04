#pragma once
#include "CJungleA2NState.h"
class CRedA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRedA2NState();
    ~CRedA2NState();
};

