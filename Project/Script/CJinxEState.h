#pragma once
#include "CChampionSkillState.h"
class CJinxEState :
    public CChampionSkillState
{
public:
    CJinxEState();
    ~CJinxEState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

