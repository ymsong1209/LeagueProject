#pragma once
#include "CJungleAlertState.h"
class CMurkWolfMiniAlertState :
    public CJungleAlertState
{
private:
    int         m_iAggroAnimNum;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CMurkWolfMiniAlertState();
    ~CMurkWolfMiniAlertState();
};
