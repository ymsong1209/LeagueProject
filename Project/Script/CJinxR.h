#pragma once
#include "CSkill.h"
class CJinxR :
    public CSkill
{
public:
    CJinxR();
    ~CJinxR();

private:
    CGameObject* m_JinxRObj;     // ��ų ���� ������ ������Ʈ(������) 
    CScript* m_JinxRScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel) override;
};

