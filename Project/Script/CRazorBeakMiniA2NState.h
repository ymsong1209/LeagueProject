#pragma once
#include "CJungleA2NState.h"
class CRazorBeakMiniA2NState :
    public CJungleA2NState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakMiniA2NState();
    ~CRazorBeakMiniA2NState();
};

