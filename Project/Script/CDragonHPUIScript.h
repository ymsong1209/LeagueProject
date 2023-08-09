#pragma once
#include <Engine\CScript.h>

class CDragonHPUIScript :
    public CScript
{
private:
    float m_fCurHP;
    float m_fTotalHP;
    float m_fRatio;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


public:
    CLONE(CDragonHPUIScript);

public:
    CDragonHPUIScript();
    ~CDragonHPUIScript();
};

