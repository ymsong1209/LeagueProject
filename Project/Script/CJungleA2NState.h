#pragma once
#include "CUnitState.h"
//Alert To Normal State
//애니메이션이 없을 경우에 그냥 바로 enter에 changestate idle을 해주면 된다.
//JungleA2NState을 상속받은 MonsterA2N에서도 tick을 상속받고 내용을 비워두면 됨.
class CJungleA2NState :
    public CUnitState
{
public:
    virtual void Enter();
    virtual void tick();
    virtual void Exit();
public:
    void HandleEvent(CGameEvent& event) override;

public:
    CJungleA2NState();
    ~CJungleA2NState();
};

