#pragma once
#include "CProjectileScript.h"
class CJinxRScript :
    public CProjectileScript
{
public:
    CJinxRScript();
    ~CJinxRScript();

    CLONE(CJinxRScript);

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

};

