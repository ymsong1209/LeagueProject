#pragma once
#include "CChampionSkillState.h"
class CVayneQState :
    public CChampionSkillState
{
public:
    CVayneQState();
    ~CVayneQState();

private:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

 