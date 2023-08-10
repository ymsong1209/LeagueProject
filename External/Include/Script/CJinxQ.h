#pragma once
#include "CSkill.h"
class CJinxQ :
    public CSkill
{
public:
    CJinxQ();
    ~CJinxQ();

private:
    JinxWeaponMode m_eWeaponMode;

    float       m_fSkillDamage;
    float       m_fSkillAttackSpeed;
    float       m_fSkillAttackRange;

public:
    void tick() override;

    virtual bool Use() override;

    float   GetSkillDamage() { return m_fSkillDamage; }
    float   GetSkillAttackSpeed() { return m_fSkillAttackSpeed; }
    float   GetSkillAttackRange() { return m_fSkillAttackRange; }
};

