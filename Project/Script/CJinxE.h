#pragma once
#include "CSkill.h"
class CJinxE :
    public CSkill
{
public:
    CJinxE();
    ~CJinxE();

private:
    CGameObject* m_JinxEObj;     // 스킬 사용시 생성할 오브젝트(프리팹) 
    CScript*     m_JinxEScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

