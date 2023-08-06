#pragma once
#include "CUnitState.h"
class CDragonSpawnState :
    public CUnitState
{
private:
    bool        m_bSpawnFinished;
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CDragonSpawnState();
    ~CDragonSpawnState();
};

