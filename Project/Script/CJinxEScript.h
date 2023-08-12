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
    float m_fAccTime;
    float m_fMaxTime;

    bool  m_bUsed;
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

};

