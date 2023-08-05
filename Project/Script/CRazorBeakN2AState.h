#pragma once
#include "CJungleN2AState.h"
class CRazorBeakN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakN2AState();
    ~CRazorBeakN2AState();
};

