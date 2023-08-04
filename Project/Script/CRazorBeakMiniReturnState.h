#pragma once
#include "CJungleReturnState.h"
class CRazorBeakMiniReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakMiniReturnState();
    ~CRazorBeakMiniReturnState();
};

