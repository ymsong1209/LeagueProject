#pragma once
#include "CStructureScript.h"
class CInhibitorScript :
    public CStructureScript
{
public:
    CInhibitorScript();
    ~CInhibitorScript();

private:
    float       m_fRespawnTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    float   GetRespawnTime() { return m_fRespawnTime; };

    void CheckStatus();
};

