#pragma once
#include "CChampionSkillState.h"
class CMalphiteWState :
    public CChampionSkillState
{
public:
    CMalphiteWState();
    ~CMalphiteWState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

