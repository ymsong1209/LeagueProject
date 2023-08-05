#pragma once
#include "CJungleAttackState.h"
class CRazorBeakAttackState :
    public CJungleAttackState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRazorBeakAttackState();
    ~CRazorBeakAttackState();
};

