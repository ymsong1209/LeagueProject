#pragma once
#include "CChampionSkillState.h"
class CJinxRState :
    public CChampionSkillState
{
public:
    CJinxRState();
    ~CJinxRState();

private:
    int         m_iSoundNum;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

