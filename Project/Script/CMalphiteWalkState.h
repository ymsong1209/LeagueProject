#pragma once
#include "CChampionWalkState.h"
class CMalphiteWalkState :
    public CChampionWalkState
{
public:
    CMalphiteWalkState();
    ~CMalphiteWalkState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

