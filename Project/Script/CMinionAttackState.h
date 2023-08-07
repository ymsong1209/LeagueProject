#pragma once
#include "CUnitState.h"
class CMinionAttackState :
    public CUnitState
{
public:
    CMinionAttackState();
    ~CMinionAttackState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

