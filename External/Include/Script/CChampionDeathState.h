#pragma once
#include "CUnitState.h"

class CChampionDeathState :
    public CUnitState
{
public:
    CChampionDeathState();
    ~CChampionDeathState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

