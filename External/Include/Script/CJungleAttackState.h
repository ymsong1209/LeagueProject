#pragma once
#include "CUnitState.h"
class CJungleAttackState :
    public CUnitState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;
public:
    //void HandleEvent(CGameEvent& event) override;
public:
    CJungleAttackState();
    ~CJungleAttackState();
};

