#pragma once
#include "CUnitState.h"
class CNexusSpawnState :
    public CUnitState
{
public:
    CNexusSpawnState();
    ~CNexusSpawnState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

