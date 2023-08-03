#pragma once
#include "CJungleChaseState.h"
class CGrompChaseState :
    public CJungleChaseState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CGrompChaseState();
    ~CGrompChaseState();
};

