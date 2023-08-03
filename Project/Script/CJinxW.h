#pragma once
#include "CSkill.h"
class CJinxW :
    public CSkill
{
public:
    CJinxW();
    ~CJinxW();

private:
    CGameObject*    m_JinxWObj;     // ��ų ���� ������ ������Ʈ(������) 
    CScript*        m_JinxWScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

