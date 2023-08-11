#pragma once
#include "CProjectileScript.h"

class CJinxWScript :
    public CProjectileScript
{
public:
    CJinxWScript();
    ~CJinxWScript();

    CLONE(CJinxWScript);

private:


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

};

