#pragma once
#include "CUnitState.h"
// Idle -> Alert animation실행하는 state
// n2a animation이 없고 alert animation만 있을 경우, Enter에서 바로 changestate로 alert를 하길 바람
// tick도 상속받아서 재정의해 비워두어야함
class CJungleN2AState :
    public CUnitState
{

private:
    bool        m_bDetectChampion;
public:
    virtual void Enter();
    virtual void tick();
    virtual void Exit();
public:
    //void HandleEvent(CGameEvent& event) override;
public:
    CJungleN2AState();
    ~CJungleN2AState();
};
