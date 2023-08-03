#pragma once
#include "CSkill.h"
class CBasicAttack :
    public CSkill
{
public:
    CBasicAttack();
    ~CBasicAttack();

private:

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel);
};

