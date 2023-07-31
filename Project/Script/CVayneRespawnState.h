#pragma once
#include "CChampionRespawnState.h"

class CVayneRespawnState :
    public CChampionRespawnState
{
public:
    CVayneRespawnState();
    ~CVayneRespawnState();

private:
    
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};


 


