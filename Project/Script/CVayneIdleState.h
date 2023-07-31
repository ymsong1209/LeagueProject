#pragma once
#include "CChampionIdleState.h"
class CVayneIdleState :
    public CChampionIdleState
{
public:
    CVayneIdleState();
    ~CVayneIdleState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

 

