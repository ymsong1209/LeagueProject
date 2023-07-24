#pragma once
#include "CUnitState.h"

class CChampionWalkState :
    public CUnitState
{
public:
    CChampionWalkState();
    virtual ~CChampionWalkState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

