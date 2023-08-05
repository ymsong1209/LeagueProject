#pragma once
#include "CJungleSpawnState.h"
class CMurkWolfSpawnState :
    public CJungleSpawnState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CMurkWolfSpawnState();
    ~CMurkWolfSpawnState();
};

