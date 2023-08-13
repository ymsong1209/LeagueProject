#pragma once
#include  <Engine/CScript.h>

class CMalphiteBasicAttackHitEffectScript :
    public CScript
{
private:
    bool m_bStartingPoint;


public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CMalphiteBasicAttackHitEffectScript);


public:
    CMalphiteBasicAttackHitEffectScript();
    ~CMalphiteBasicAttackHitEffectScript();
};

