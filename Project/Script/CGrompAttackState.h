#pragma once
#include "CJungleAttackState.h"
class CGrompAttackState :
    public CJungleAttackState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CGrompAttackState();
    ~CGrompAttackState();
};

