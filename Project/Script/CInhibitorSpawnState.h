#pragma once
#include "CUnitState.h"
class CInhibitorSpawnState :
    public CUnitState
{
public:
    CInhibitorSpawnState();
    ~CInhibitorSpawnState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

};

