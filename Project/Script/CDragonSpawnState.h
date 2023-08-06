#pragma once
#include "CUnitState.h"
class CDragonSpawnState :
    public CUnitState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CDragonSpawnState();
    ~CDragonSpawnState();
};

