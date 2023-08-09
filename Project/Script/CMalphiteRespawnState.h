#pragma once
#include "CChampionRespawnState.h"
class CMalphiteRespawnState :
    public CChampionRespawnState
{
public:
    CMalphiteRespawnState();
    ~CMalphiteRespawnState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

