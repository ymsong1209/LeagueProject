#pragma once
#include "CUnitState.h"
class CInhibitorRespawnState :
    public CUnitState
{
public:
    CInhibitorRespawnState();
    ~CInhibitorRespawnState();

private:
    float   m_fRespawnTime;
    float   m_fDeadTime;
    bool    m_bSounded;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

