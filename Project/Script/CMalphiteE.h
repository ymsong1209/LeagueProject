#pragma once
#include "CSkill.h"
class CMalphiteE :
    public CSkill
{
public:
    CMalphiteE();
    ~CMalphiteE();

private:
    CGameObject* m_JinxWObj;     // 스킬 사용시 생성할 오브젝트(프리팹) 
    CScript* m_JinxWScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

