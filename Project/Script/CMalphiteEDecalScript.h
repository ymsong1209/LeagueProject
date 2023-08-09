#pragma once
#include <Engine/CScript.h>
class CMalphiteEDecalScript :
    public CScript
{
private:
    CGameObject* m_pOwner;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMalphiteEDecalScript)
public:
    CMalphiteEDecalScript();
    ~CMalphiteEDecalScript();
};

