#pragma once
#include "CChampionSkillState.h"
class CMalphiteQState :
    public CChampionSkillState
{
public:
    CMalphiteQState();
    ~CMalphiteQState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

