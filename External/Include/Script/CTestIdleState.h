#pragma once
#include <Engine/CState.h>

class CTestIdleState :
    public CState
{

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CTestIdleState();
    ~CTestIdleState();

};

