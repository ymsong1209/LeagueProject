#pragma once
#include "CProjectileScript.h"
class CRangedMinionBasicAttackScript :
    public CProjectileScript
{
public:
    CRangedMinionBasicAttackScript();
    ~CRangedMinionBasicAttackScript();

    CLONE(CRangedMinionBasicAttackScript);

private:
    SkillType   m_skillType;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    void SetSkillType(SkillType _type) { m_skillType = _type; }
};

