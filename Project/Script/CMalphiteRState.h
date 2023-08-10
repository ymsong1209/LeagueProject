#pragma once
#include "CChampionSkillState.h"
class CMalphiteRState :
    public CChampionSkillState
{
private:
    Vec3        m_vTargetPos;
    float       m_fSpeed;

public:
    void SetTargetPos(Vec3 _pos) { m_vTargetPos = _pos; }
public:
    CMalphiteRState();
    ~CMalphiteRState();
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

