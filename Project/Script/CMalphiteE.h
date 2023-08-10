#pragma once
#include "CSkill.h"
class CMalphiteE :
    public CSkill
{
public:
    CMalphiteE();
    ~CMalphiteE();

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

