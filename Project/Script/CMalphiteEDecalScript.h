#pragma once
#include <Engine/CScript.h>
class CMalphiteEDecalScript :
    public CScript
{
private:
    float       m_fTime;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMalphiteEDecalScript)

public:
 
public:
    CMalphiteEDecalScript();
    ~CMalphiteEDecalScript();
};

