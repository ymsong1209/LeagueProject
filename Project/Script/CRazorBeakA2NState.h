#pragma once
#include "CJungleA2NState.h"
class CRazorBeakA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakA2NState();
    ~CRazorBeakA2NState();
};

