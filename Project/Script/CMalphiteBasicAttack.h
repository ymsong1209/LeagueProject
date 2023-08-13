#pragma once
#include "CBasicAttack.h"
class CMalphiteBasicAttack :
    public CBasicAttack
{
public:
    CMalphiteBasicAttack();
    ~CMalphiteBasicAttack();

private:

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel);
};

