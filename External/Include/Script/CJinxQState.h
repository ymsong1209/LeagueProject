#pragma once
#include "CChampionSkillState.h"
class CJinxQState :
    public CChampionSkillState
{
public:
    CJinxQState();
    ~CJinxQState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

