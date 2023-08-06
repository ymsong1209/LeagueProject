#pragma once
#include <Engine/CScript.h>

class CAttackRangeScript :
    public CScript
{
   
public:
    virtual void begin() override;

    CAttackRangeScript();
    ~CAttackRangeScript();

    CLONE(CAttackRangeScript);

private:
    vector<CGameObject*>    m_vecUnitsInRange;

public:
    virtual void BeginOverlap(CCollider2D* _Other);
    virtual void EndOverlap(CCollider2D* _Other);


public:
    vector<CGameObject*>& GetUnitsInRange() { return m_vecUnitsInRange; };
};

