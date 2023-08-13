#pragma once
#include "CUnitState.h"
class CDragonChaseState :
    public CUnitState
{
private:
    float       m_fTime;
    float       m_fSoundTime;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    void HandleEvent(CGameEvent& event) override;


public:
    CDragonChaseState();
    ~CDragonChaseState();
};

