#pragma once
#include "CJungleAlertState.h"
class CRazorBeakAlertState :
    public CJungleAlertState
{
private:
    int         m_iAggroAnimNum;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CRazorBeakAlertState();
    ~CRazorBeakAlertState();
};
