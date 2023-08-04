#pragma once
#include "CJungleAttackState.h"
class CRazorBeakMiniAttackState :
    public CJungleAttackState
{
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CRazorBeakMiniAttackState();
    ~CRazorBeakMiniAttackState();
};

