#pragma once
#include "CJungleSpawnState.h"
class CGrompSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CGrompSpawnState();
    ~CGrompSpawnState();
};

