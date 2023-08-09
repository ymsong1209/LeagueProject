#pragma once
#include "CSkill.h"
class CJinxR :
    public CSkill
{
public:
    CJinxR();
    ~CJinxR();

private:
    CGameObject* m_JinxRObj;     // 스킬 사용시 생성할 오브젝트(프리팹) 
    CScript* m_JinxRScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

