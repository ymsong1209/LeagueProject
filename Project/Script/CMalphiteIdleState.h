#pragma once
#include "CChampionIdleState.h"
class CMalphiteIdleState :
    public CChampionIdleState
{
private:
    int         m_iIdleAnimNum;
public:
    CMalphiteIdleState();
    ~CMalphiteIdleState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

