#pragma once
#include "CSkill.h"
class CVayneW :
    public CSkill
{
public:
    CVayneW();
    ~CVayneW();

private:
    CGameObject*  m_VayneWObj;     // 스킬 사용시 생성할 오브젝트(프리팹) 
    CScript*  m_VayneWScript;

public:
    virtual void tick() override;
    virtual bool   Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript) override;
};

 

