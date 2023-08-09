#pragma once
#include "CSkill.h"
class CJinxE :
    public CSkill
{
public:
    CJinxE();
    ~CJinxE();

private:
    CGameObject* m_JinxEObj;     // ��ų ���� ������ ������Ʈ(������) 
    CScript*     m_JinxEScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

