#include "pch.h"
#include "CSkillMgr.h"
#include "CSkill.h"
#include "CBasicAttack.h"
#include "CBasicAttackScript.h"
#include "CJinxWScript.h"
#include "CJinxEScript.h"
#include "CJinxRScript.h"

#include "CMalphiteBasicAttackScript.h"
#include "CMalphiteQScript.h"
#include "CMalphiteEScript.h"
#include "CMalphiteRScript.h"

#include "CJinxW.h"
#include "CJinxE.h"
#include "CJinxR.h"

#include "CMalphiteBasicAttack.h"
#include "CMalphiteQ.h"
#include "CMalphiteW.h"
#include "CMalphiteE.h"
#include "CMalphiteR.h"

CSkillMgr::CSkillMgr()
{
    CBasicAttack* BasicAttack = new CBasicAttack;
    m_mapSkills.insert(make_pair(SkillType::BASIC_ATTACK, BasicAttack));

    CMalphiteBasicAttack* MalphiteBasicAttack = new CMalphiteBasicAttack;
    m_mapSkills.insert(make_pair(SkillType::MALPHITE_BASIC_ATTACK, MalphiteBasicAttack));

    CJinxW* JinxW = new CJinxW;
    m_mapSkills.insert(make_pair(SkillType::JINX_W, JinxW));

    CJinxE* JinxE = new CJinxE;
    m_mapSkills.insert(make_pair(SkillType::JINX_E, JinxE));

    CJinxR* JinxR = new CJinxR;
    m_mapSkills.insert(make_pair(SkillType::JINX_R, JinxR));

    CMalphiteQ* malphQ = new CMalphiteQ;
    m_mapSkills.insert(make_pair(SkillType::MALPHITE_Q, malphQ));

    CMalphiteW* malphW = new CMalphiteW;
    m_mapSkills.insert(make_pair(SkillType::MALPHITE_W, malphW));

    CMalphiteE* malphE = new CMalphiteE;
    m_mapSkills.insert(make_pair(SkillType::MALPHITE_E, malphE));

    CMalphiteR* malphR = new CMalphiteR;
    m_mapSkills.insert(make_pair(SkillType::MALPHITE_R, malphR));
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
    {
        CBasicAttackScript* BasicAttackScript = new CBasicAttackScript;
        BasicAttackScript->SetSkillType(SkillType::BASIC_ATTACK);
        return BasicAttackScript;
    }
        break;
    case SkillType::MALPHITE_BASIC_ATTACK:
    {
        CBasicAttackScript* BasicAttackScript = new CBasicAttackScript;
        BasicAttackScript->SetSkillType(SkillType::MALPHITE_BASIC_ATTACK);
        return BasicAttackScript;
    }
        break;
    case SkillType::JINX_W:
        return new CJinxWScript;
        break;
    case SkillType::JINX_E:
        return new CJinxEScript;
        break;
    case SkillType::JINX_R:
        return new CJinxRScript;
        break;
    case SkillType::DARIUS_Q:
        break;
    case SkillType::DARIUS_W:
        break;
    case SkillType::DARIUS_E:
        break;
    case SkillType::DARIUS_R:
        break;
    case SkillType::MALPHITE_Q:
        return new CMalphiteQScript;
        break;
    case SkillType::MALPHITE_W:
        
        break;
    case SkillType::MALPHITE_E:
        return new CMalphiteEScript;
        break;
    case SkillType::MALPHITE_R:
        return new CMalphiteRScript;
        break;
    }
}


