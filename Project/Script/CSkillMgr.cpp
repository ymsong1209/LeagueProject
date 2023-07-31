#include "pch.h"
#include "CSkillMgr.h"
#include "CSkill.h"
#include "CBaseAttack.h"

CSkillMgr::CSkillMgr()
{
    CBaseAttack* BaseAttack = new CBaseAttack;
    m_mapSkills.insert(make_pair(SkillType::BASIC_ATTACK, BaseAttack));
}

CSkillMgr::~CSkillMgr()
{
    Safe_Del_Map(m_mapSkills);
}



CSkill* CSkillMgr::FindSkill(SkillType _type)
{
    for (auto it = m_mapSkills.begin(); it != m_mapSkills.end(); )
    {
        if (_type == it->first)
        {
            return it->second; 
        }
    }
    return nullptr; 
}

