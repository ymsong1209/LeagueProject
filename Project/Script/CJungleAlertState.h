#pragma once
#include "CUnitState.h"

class CJungleAlertState :
    public CUnitState
{
public:
    CJungleAlertState();
    virtual ~CJungleAlertState();

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

protected:
    void HandleEvent(CGameEvent& event) override;
};

