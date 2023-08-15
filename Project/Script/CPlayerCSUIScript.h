#pragma once
#include <Engine\CScript.h>

class CPlayerCSUIScript :
    public CScript
{
private:
    float m_fTimer;
    bool m_bAnimating;
    float animationDuration;
    float animationDistance;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CPlayerCSUIScript);

public:
    CPlayerCSUIScript();
    ~CPlayerCSUIScript();
};

