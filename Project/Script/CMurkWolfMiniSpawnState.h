#pragma once
#include "CJungleSpawnState.h"
class CMurkWolfMiniSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CMurkWolfMiniSpawnState();
    ~CMurkWolfMiniSpawnState();
};

