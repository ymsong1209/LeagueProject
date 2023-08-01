#pragma once
#include "CChampionSkillState.h"
class CJinxWState :
    public CChampionSkillState
{
public:
    CJinxWState();
    ~CJinxWState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

