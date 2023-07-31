#pragma once
#include "CSkill.h"
class CBaseAttack :
    public CSkill
{
public:
    CBaseAttack();
    ~CBaseAttack();

private:
    UINT64  m_iServerID;


public:
    virtual void tick() override;
    virtual bool Use() override;

    virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript);

    void SetServerID(UINT64 _i) { m_iServerID = _i; }
    UINT64 GetServerID() { return m_iServerID; }
};

