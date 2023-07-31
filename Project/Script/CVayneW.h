#pragma once
#include "CSkill.h"
class CVayneW :
    public CSkill
{
public:
    CVayneW();
    ~CVayneW();

private:
    CGameObject*  m_VayneWObj;     // ��ų ���� ������ ������Ʈ(������) 
    CScript*  m_VayneWScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript) override;
};

 

