#pragma once
#include "CUnitState.h"
class CChampionSkillState :
    public CUnitState
{
public:
    CChampionSkillState();
    ~CChampionSkillState();

protected:
    float       m_fCastingTime;
    float       m_fCurCastingTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

