#pragma once
#include <Engine\CScript.h>

class CJungleMobHPScript :
    public CScript
{
private:
    float m_fCurHP;
    float m_fTotalHP;
    float m_fRatio;

    Vec2 OffsetFontPos;
    Vec2 OffsetBarPos;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


public:
    CLONE(CJungleMobHPScript);

public:
    CJungleMobHPScript();
    ~CJungleMobHPScript();
};

