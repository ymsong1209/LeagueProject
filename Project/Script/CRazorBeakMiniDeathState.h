#pragma once
#include "CJungleDeathState.h"
class CRazorBeakMiniDeathState :
    public CJungleDeathState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRazorBeakMiniDeathState();
    ~CRazorBeakMiniDeathState();
};
