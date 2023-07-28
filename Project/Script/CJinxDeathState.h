#pragma once
#include "CChampionDeathState.h"
class CJinxDeathState :
    public CChampionDeathState
{
public:
    CJinxDeathState();
    ~CJinxDeathState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

