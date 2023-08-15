#pragma once
#include <Engine/CScript.h>
class CMinionBasicAttackHitEffectScript :
    public CScript
{
private:
    bool m_bStartingPoint;

public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CMinionBasicAttackHitEffectScript);

public:
    CMinionBasicAttackHitEffectScript();
    ~CMinionBasicAttackHitEffectScript();
};
 
 
