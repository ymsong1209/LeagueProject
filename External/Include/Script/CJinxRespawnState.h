#pragma once
#include "CChampionRespawnState.h"
class CJinxRespawnState :
    public CChampionRespawnState
{
public:
    CJinxRespawnState();
    ~CJinxRespawnState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

