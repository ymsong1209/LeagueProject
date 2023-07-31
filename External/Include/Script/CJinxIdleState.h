#pragma once
#include "CChampionIdleState.h"
class CJinxIdleState :
    public CChampionIdleState
{
public:
    CJinxIdleState();
    ~CJinxIdleState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

