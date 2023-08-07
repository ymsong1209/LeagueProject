
#pragma once
#include "CUnitState.h"
class CDragonAlert2AggroState :
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
    CDragonAlert2AggroState();
    ~CDragonAlert2AggroState();
};

