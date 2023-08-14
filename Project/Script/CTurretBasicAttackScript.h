#pragma once
#include "CProjectileScript.h"
class CTurretBasicAttackScript :
    public CProjectileScript
{
public:
    CTurretBasicAttackScript();
    ~CTurretBasicAttackScript();

    CLONE(CTurretBasicAttackScript);

private:
    SkillType   m_skillType;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    void SetSkillType(SkillType _type) { m_skillType = _type; }
};

