#pragma once
#include  <Engine/CScript.h>

class CMalphiteBasicAttackScript :
    public CScript
{
private:
    bool m_bStartingPoint;


public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CMalphiteBasicAttackScript);


public:
    CMalphiteBasicAttackScript();
    ~CMalphiteBasicAttackScript();
};

