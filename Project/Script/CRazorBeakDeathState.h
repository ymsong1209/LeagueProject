#pragma once
#include "CJungleDeathState.h"
class CRazorBeakDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRazorBeakDeathState();
    ~CRazorBeakDeathState();
};

