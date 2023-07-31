#pragma once
#include "CUnitState.h"
class CInhibitorBrokenState :
    public CUnitState
{
public:
    CInhibitorBrokenState();
    ~CInhibitorBrokenState();

private:
    float   m_fRespawnTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
};

