#pragma once
#include "CChampionWalkState.h"
class CJinxWalkState :
    public CChampionWalkState
{
public:
    CJinxWalkState();
    ~CJinxWalkState();

private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    void HandleEvent(CGameEvent& event) override;
};

