#pragma once
#include "CProjectileScript.h"

class CBasicAttackScript :
    public CProjectileScript
{
public:
    CBasicAttackScript();
    ~CBasicAttackScript();

    CLONE(CBasicAttackScript);

private:
    SkillType   m_skillType;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    void SetSkillType(SkillType _type) { m_skillType = _type; }
};

