
#pragma once
#include <Engine/CScript.h>
class CMalphiteRDecalScript :
    public CScript
{
private:
    float       m_fTime;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMalphiteRDecalScript)

public:

public:
    CMalphiteRDecalScript();
    ~CMalphiteRDecalScript();
};

