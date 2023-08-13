#pragma once
#include "CProjectileScript.h"

class CBasicAttackScript :
    public CProjectileScript
{
public:
    CBasicAttackScript();
    ~CBasicAttackScript();

    CLONE(CBasicAttackScript);

private:

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;
};

