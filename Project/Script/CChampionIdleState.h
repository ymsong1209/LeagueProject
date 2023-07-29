#pragma once
#include "CUnitState.h"

class CChampionIdleState :
    public CUnitState
{
public:
    CChampionIdleState();
    virtual ~CChampionIdleState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

