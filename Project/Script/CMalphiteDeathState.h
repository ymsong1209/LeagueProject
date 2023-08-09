#pragma once
#include "CChampionDeathState.h"
class CMalphiteDeathState :
    public CChampionDeathState
{
public:
    CMalphiteDeathState();
    ~CMalphiteDeathState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

