#pragma once
#include "CJungleReturnState.h"
class CRedReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRedReturnState();
    ~CRedReturnState();
};

