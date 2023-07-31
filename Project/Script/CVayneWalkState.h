#pragma once
#include "CChampionWalkState.h"
class CVayneWalkState :
    public CChampionWalkState
{
public:
    CVayneWalkState();
    ~CVayneWalkState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};
 
