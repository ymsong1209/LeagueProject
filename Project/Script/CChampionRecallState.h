#pragma once
#include "CUnitState.h"

class CChampionRecallState :
    public CUnitState
{
public:
    CChampionRecallState();
    ~CChampionRecallState();

protected:
    float       m_fRecallTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

