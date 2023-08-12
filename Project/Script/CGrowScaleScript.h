#pragma once
#include <Engine/CScript.h>

class CGrowScaleScript :
    public CScript
{
public:
    Vec3  m_vDirection;
    float m_fScaleSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CGrowScaleScript);

public:
    CGrowScaleScript();
    ~CGrowScaleScript();

};

