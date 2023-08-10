#pragma once
#include "CProjectileScript.h"
class CJinxEScript :
    public CProjectileScript
{
public:
    CJinxEScript();
    ~CJinxEScript();

    CLONE(CJinxEScript);

private:

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

};

