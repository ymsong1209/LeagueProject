#pragma once
#include "CChampionAttackState.h"

class CMalphiteAttackState :
    public CChampionAttackState
{
private:
    int         m_iAttackAnimNum;
    int         m_iMalphiteSoundNum;
public:
    CMalphiteAttackState();
    ~CMalphiteAttackState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

