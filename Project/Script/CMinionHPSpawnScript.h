#pragma once
#include <Engine\CScript.h>

class CMinionHPSpawnScript :
    public CScript
{
private:
    Faction OwnerFaction;
    CGameObject* MinionBar;
    bool        m_bInit; 
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


    CLONE(CMinionHPSpawnScript);

public:
    CMinionHPSpawnScript();
    ~CMinionHPSpawnScript();
};

