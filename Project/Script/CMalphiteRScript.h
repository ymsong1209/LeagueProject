#pragma once
#include "CProjectileScript.h"

class CMalphiteRScript :
    public CProjectileScript
{
private:
    float           m_fTime;

public:
    CMalphiteRScript();
    ~CMalphiteRScript();

    CLONE(CMalphiteRScript);

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

};

