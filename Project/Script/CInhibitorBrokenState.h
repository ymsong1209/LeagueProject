#pragma once
#include "CUnitState.h"
class CInhibitorBrokenState :
    public CUnitState
{
public:
    CInhibitorBrokenState();
    ~CInhibitorBrokenState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

