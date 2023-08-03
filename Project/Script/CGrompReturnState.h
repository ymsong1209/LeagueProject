#pragma once
#include "CJungleReturnState.h"
class CGrompReturnState :
    public CJungleReturnState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CGrompReturnState();
    ~CGrompReturnState();
};

