#pragma once
#include "CJungleReturnState.h"
class CRazorBeakReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakReturnState();
    ~CRazorBeakReturnState();
};

