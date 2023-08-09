#pragma once
#include "CUnitState.h"
class CJungleChaseState :
    public CUnitState
{
private:
    float       m_fTime;
    Vec3        m_vTargetPos;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;

public:
    CJungleChaseState();
    ~CJungleChaseState();
};

