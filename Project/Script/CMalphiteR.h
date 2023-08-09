#pragma once
#include "CSkill.h"
class CMalphiteR :
    public CSkill
{
public:
    CMalphiteR();
    ~CMalphiteR();

private:
    CGameObject* m_MalphRProjectile;
    CGameObject* m_MalphDecal;


    CScript* m_JinxWScript;

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

