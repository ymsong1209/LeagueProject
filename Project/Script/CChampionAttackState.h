#pragma once
#include "CUnitState.h"

class CChampionAttackState :
    public CUnitState
{
public:
    CChampionAttackState();
    ~CChampionAttackState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

