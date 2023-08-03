#pragma once
#include <Engine/CScript.h>
//#include "CUnitScript.h"
class CTurretAttackScript :
    public CScript
{
private:
    float           m_fSpeed;
    CGameObject*    m_pTarget;
    Vec3            m_vTargetPos;

    bool            m_bPosChase;

public:
    void SetTarget(CGameObject* _target) { m_pTarget = _target; }
public:
    virtual void tick() override;
    virtual void begin() override;
    virtual void BeginOverlap(CCollider3D* _Other) override;
    CLONE(CTurretAttackScript);
   
public:
    CTurretAttackScript();
    ~CTurretAttackScript();
};

