#pragma once
#include "CJungleReturnState.h"
class CBlueReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CBlueReturnState();
    ~CBlueReturnState();
};

