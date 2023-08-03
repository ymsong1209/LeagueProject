#pragma once
#include "CUnitState.h"

class CJungleSpawnState :
    public CUnitState
{
public:
    CJungleSpawnState();
    virtual ~CJungleSpawnState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

