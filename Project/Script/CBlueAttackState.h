#pragma once
#include "CJungleAttackState.h"
class CBlueAttackState :
    public CJungleAttackState
{
private:
    int         m_iAttackAnimNum;
public:
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void tick() override;

public:
    CBlueAttackState();
    ~CBlueAttackState();
};

