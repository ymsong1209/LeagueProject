#pragma once
#include <Engine\CScript.h>

class CJungleMINIHPScript :
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


public:
    CLONE(CJungleMINIHPScript);

public:
    CJungleMINIHPScript();
    ~CJungleMINIHPScript();
};

