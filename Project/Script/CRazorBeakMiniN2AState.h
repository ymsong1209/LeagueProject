#pragma once
#include "CJungleN2AState.h"
class CRazorBeakMiniN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakMiniN2AState();
    ~CRazorBeakMiniN2AState();
};

