#pragma once
#include "CChampionDeathState.h"
class CVayneDeathState :
    public CChampionDeathState
{
public:
    CVayneDeathState();
    ~CVayneDeathState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

 

