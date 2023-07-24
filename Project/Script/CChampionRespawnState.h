#pragma once
#include "CUnitState.h"
class CChampionRespawnState :
    public CUnitState
{
public:
    CChampionRespawnState();
    ~CChampionRespawnState();

protected:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

