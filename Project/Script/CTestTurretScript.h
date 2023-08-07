#pragma once
#include <Engine/CScript.h>
class CTestTurretScript :
    public CScript
{
public:
    virtual void tick() override;
    virtual void begin() override;

    CLONE(CTestTurretScript);
    
public:
    CTestTurretScript();
    ~CTestTurretScript();
};

 