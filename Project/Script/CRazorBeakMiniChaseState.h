#pragma once
#include "CJungleChaseState.h"
class CRazorBeakMiniChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakMiniChaseState();
    ~CRazorBeakMiniChaseState();
};

