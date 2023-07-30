#pragma once
#include "CChampionAttackState.h"

class CJinxAttackState :
    public CChampionAttackState
{
public:
    CJinxAttackState();
    ~CJinxAttackState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

