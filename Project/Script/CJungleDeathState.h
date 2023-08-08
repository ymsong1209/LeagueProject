#pragma once
#include "CUnitState.h"
class CJungleDeathState :
    public CUnitState
{
private:
    bool        m_bPacketSend;
public:
    virtual void Enter() override;
    virtual void tick() override;
    virtual void Exit() override;

public:
    CJungleDeathState();
    ~CJungleDeathState();
};

