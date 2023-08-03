#include "pch.h"
#include "CSkillMgr.h"
#include "CSkill.h"
#include "CBasicAttack.h"
#include "CBasicAttackScript.h"
#include "CJinxWScript.h"

#include "CJinxW.h"

CSkillMgr::CSkillMgr()
{
    CBasicAttack* BasicAttack = new CBasicAttack;
    m_mapSkills.insert(make_pair(SkillType::BASIC_ATTACK, BasicAttack));

    CJinxW* JinxW = new CJinxW;
    m_mapSkills.insert(make_pair(SkillType::JINX_W, JinxW));


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
        else
            it++;
    }
    return nullptr; 
}

CProjectileScript* CSkillMgr::FindProjectileScript(SkillType _Type)
{
    switch (_Type)
    {
    case SkillType::BASIC_ATTACK:
        return new CBasicAttackScript;
        break;
    case SkillType::JINX_W:
        return new CJinxWScript;
        break;
    case SkillType::JINX_E:
        break;
    case SkillType::JINX_R:
        break;
    case SkillType::DARIUS_Q:
        break;
    case SkillType::DARIUS_W:
        break;
    case SkillType::DARIUS_E:
        break;
    case SkillType::DARIUS_R:
        break;
    }
}


