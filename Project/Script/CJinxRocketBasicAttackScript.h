#pragma once
#include "CProjectileScript.h"
class CJinxRocketBasicAttackScript :
    public CProjectileScript
{
public:
    CJinxRocketBasicAttackScript();
    ~CJinxRocketBasicAttackScript();

    CLONE(CJinxRocketBasicAttackScript);

private:
    SkillType   m_skillType;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other)  override;

    void SetSkillType(SkillType _type) { m_skillType = _type; }
};


