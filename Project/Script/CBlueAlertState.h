#pragma once
#include "CJungleAlertState.h"
class CBlueAlertState :
    public CJungleAlertState
{
private:
    int         m_iAggroAnimNum;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CBlueAlertState();
    ~CBlueAlertState();
};

