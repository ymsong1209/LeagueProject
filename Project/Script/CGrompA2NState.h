#pragma once
#include "CJungleA2NState.h"
class CGrompA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CGrompA2NState();
    ~CGrompA2NState();
};

