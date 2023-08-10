#pragma once
#include "CSkill.h"

class CMalphiteQ :
    public CSkill
{
public:
    CMalphiteQ();
    ~CMalphiteQ();

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

