#pragma once
#include "CChampionAttackState.h"

class CTeemoAttackState :
    public CChampionAttackState
{
private:
    int         m_iAttackAnimNum;
public:
    CTeemoAttackState();
    ~CTeemoAttackState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

