#pragma once
#include "CUnitState.h"
class CDragonLandState :
    public CUnitState
{
private:

public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;
public:
    void HandleEvent(CGameEvent& event) override;
public:
    CDragonLandState();
    ~CDragonLandState();
};

