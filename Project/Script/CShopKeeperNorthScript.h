#pragma once
#include <Engine/CScript.h>
class CShopKeeperNorthScript :
    public CScript
{
private:
    int m_iAnimNum;
public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CShopKeeperNorthScript);
public:
    CShopKeeperNorthScript();
    ~CShopKeeperNorthScript();
};

