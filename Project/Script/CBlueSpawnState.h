#pragma once
#include "CJungleSpawnState.h"
class CBlueSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CBlueSpawnState();
    ~CBlueSpawnState();
};

