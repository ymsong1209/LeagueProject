#pragma once
#include <Engine/CSingleton.h>

class CSkill;
class CProjectileScript;

class CSkillMgr :
    public CSingleton<CSkillMgr>
{
    SINGLE(CSkillMgr);

private:
    map <SkillType, CSkill*>    m_mapSkills;

public:
    CSkill* FindSkill(SkillType _type);
    CProjectileScript* FindProjectileScript(SkillType _Type);
};

