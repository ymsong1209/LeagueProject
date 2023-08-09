#pragma once
#include "CSkill.h"
class CMalphiteQ :
    public CSkill
{
public:
    CMalphiteQ();
    ~CMalphiteQ();

private:
    CGameObject* m_MalphRProjectile;
    CGameObject* m_MalphDecal;


    CScript* m_JinxWScript;

public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

