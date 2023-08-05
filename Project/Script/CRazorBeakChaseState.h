#pragma once
#include "CJungleChaseState.h"
class CRazorBeakChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakChaseState();
    ~CRazorBeakChaseState();
};

