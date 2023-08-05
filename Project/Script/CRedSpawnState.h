#pragma once
#include "CJungleSpawnState.h"
class CRedSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRedSpawnState();
    ~CRedSpawnState();
};

