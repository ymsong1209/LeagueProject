#pragma once
#include "CUnitState.h"
class CDragonAttackState :
    public CUnitState
{
private:
    float        m_fAttackCoolTime;
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    void HandleEvent(CGameEvent& event) override;

public:
    CDragonAttackState();
    ~CDragonAttackState();
};

