#pragma once
#include "CUnitState.h"
class CInhibitorRespawnState :
    public CUnitState
{
public:
    CInhibitorRespawnState();
    ~CInhibitorRespawnState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

