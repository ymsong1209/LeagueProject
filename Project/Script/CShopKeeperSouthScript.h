#pragma once
#include <Engine/CScript.h>
class CShopKeeperSouthScript :
    public CScript
{
private:
    int m_iAnimNum;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CShopKeeperSouthScript);
public:
    CShopKeeperSouthScript();
    ~CShopKeeperSouthScript();
};

