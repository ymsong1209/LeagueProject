#pragma once
#include "CProjectileScript.h"

class CMalphiteEScript :
    public CProjectileScript
{
private:
    float           m_fTime;

public:
    CMalphiteEScript();
    ~CMalphiteEScript();

    CLONE(CMalphiteEScript);

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

};

