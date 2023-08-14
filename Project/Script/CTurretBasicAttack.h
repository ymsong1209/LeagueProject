#pragma once
#include "CBasicAttack.h"

class CTurretBasicAttack :
    public CBasicAttack
{
public:
    CTurretBasicAttack();
    ~CTurretBasicAttack();

private:

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel);
};
