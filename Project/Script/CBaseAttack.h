#pragma once
#include "CSkill.h"
class CBaseAttack :
    public CSkill
{
public:
    CBaseAttack();
    ~CBaseAttack();

private:

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript);
};

