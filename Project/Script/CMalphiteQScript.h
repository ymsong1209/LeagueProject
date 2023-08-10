#pragma once
#include "CProjectileScript.h"

class CMalphiteQScript :
    public CProjectileScript
{
private:
    float           m_fTime;
    CGameObject*    m_pMalphiteQDecal;
    CGameObject*    m_pMalphiteQModel;
public:
    CMalphiteQScript();
    ~CMalphiteQScript();

    CLONE(CMalphiteQScript);

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    virtual void Despawn() override;

};

