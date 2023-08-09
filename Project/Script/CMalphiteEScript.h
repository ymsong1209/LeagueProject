#pragma once
#include "CProjectileScript.h"

class CMalphiteEScript :
    public CProjectileScript
{
public:
    CMalphiteEScript();
    ~CMalphiteEScript();

    CLONE(CMalphiteEScript);

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void OnOverlap(CCollider2D* _Other)  override;

};

