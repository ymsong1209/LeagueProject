#pragma once
#include "CJungleN2AState.h"
class CGrompN2AState :
    public CJungleN2AState
{
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CGrompN2AState();
    ~CGrompN2AState();
};

