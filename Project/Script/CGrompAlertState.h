#pragma once
#include "CJungleAlertState.h"
class CGrompAlertState :
    public CJungleAlertState
{
private:
    int         m_iAggroAnimNum;
    bool        m_bEnterAnimPlayed;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CGrompAlertState();
    ~CGrompAlertState();
};

