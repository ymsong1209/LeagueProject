#pragma once
#include "CJungleDeathState.h"
class CDragonDeathState :
    public CUnitState
{
private:
    bool    m_bPacketSend;
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CDragonDeathState();
    ~CDragonDeathState();
};

