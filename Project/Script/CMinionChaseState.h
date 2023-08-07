#pragma once
#include "CUnitState.h"
class CMinionChaseState :
    public CUnitState
{
public:
    CMinionChaseState();
    ~CMinionChaseState();

private:
    float       m_fAggroTime;
    float       m_fTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void HandleEvent(CGameEvent& event);
};

