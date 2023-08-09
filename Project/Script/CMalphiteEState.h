#pragma once
#include "CChampionSkillState.h"
class CMalphiteEState :
    public CChampionSkillState
{
public:
    CMalphiteEState();
    ~CMalphiteEState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

