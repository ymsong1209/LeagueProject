#include "pch.h"
#include "CSkillMgr.h"
#include "CSkill.h"

#include "CBasicAttack.h"
#include "CRangedMinionBasicAttack.h"
#include "CBasicAttackScript.h"
#include "CRangedMinionBasicAttackScript.h"

#include "CDragonBasicAttack.h"
#include "CDragonBasicAttackScript.h"

#include "CTurretBasicAttack.h"
#include "CTurretBasicAttackScript.h"

#include "CJinxBasicAttack.h"
#include "CJinxRocketBasicAttack.h"
#include "CJinxWScript.h"
#include "CJinxEScript.h"
#include "CJinxRScript.h"

#include "CJinxBasicAttackScript.h"
#include "CJinxRocketBasicAttackScript.h"
#include "CJinxW.h"
#include "CJinxE.h"
#include "CJinxR.h"

#include "CMalphiteBasicAttack.h"
#include "CMalphiteQ.h"
#include "CMalphiteW.h"
#include "CMalphiteE.h"
#include "CMalphiteR.h"
#include "CMalphiteQScript.h"
#include "CMalphiteEScript.h"
#include "CMalphiteRScript.h"


CSkillMgr::CSkillMgr()
{
	CBasicAttack* BasicAttack = new CBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::BASIC_ATTACK, BasicAttack));

	CRangedMinionBasicAttack* RangedMinionBasicAttack = new CRangedMinionBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::BASIC_RANGED_ATTACK, RangedMinionBasicAttack));

	CDragonBasicAttack* DragonBasicAttack = new CDragonBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::DRAGON_BASIC_ATTACK, DragonBasicAttack));

	CTurretBasicAttack* TurretBasicAttack = new CTurretBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::TURRET_BASIC_ATTACK, TurretBasicAttack));

	CMalphiteBasicAttack* MalphiteBasicAttack = new CMalphiteBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::MALPHITE_BASIC_ATTACK, MalphiteBasicAttack));

	CJinxBasicAttack* JinxBasicAttack = new CJinxBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::JINX_BASIC_ATTACK, JinxBasicAttack));

	CJinxRocketBasicAttack* JinxRocketBasicAttack = new CJinxRocketBasicAttack;
	m_mapSkills.insert(make_pair(SkillType::JINX_ROCKET_BASIC_ATTACK, JinxRocketBasicAttack));

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
	case SkillType::BASIC_RANGED_ATTACK:
	{
		CRangedMinionBasicAttackScript* RangedMinionBasicAttackScript = new CRangedMinionBasicAttackScript;
		RangedMinionBasicAttackScript->SetSkillType(SkillType::BASIC_RANGED_ATTACK);
		return RangedMinionBasicAttackScript;
	}
	break;
	case SkillType::DRAGON_BASIC_ATTACK:
	{
		CDragonBasicAttackScript* DragonBasicAttackScript = new CDragonBasicAttackScript;
		DragonBasicAttackScript->SetSkillType(SkillType::DRAGON_BASIC_ATTACK);
		return DragonBasicAttackScript;
	}
	break;
	case SkillType::TURRET_BASIC_ATTACK:
	{
		CTurretBasicAttackScript* TurretBasicAttackScript = new CTurretBasicAttackScript;
		TurretBasicAttackScript->SetSkillType(SkillType::TURRET_BASIC_ATTACK);
		return TurretBasicAttackScript;
	}
	break;
	case SkillType::MALPHITE_BASIC_ATTACK:
	{
		CBasicAttackScript* BasicAttackScript = new CBasicAttackScript;
		BasicAttackScript->SetSkillType(SkillType::MALPHITE_BASIC_ATTACK);
		return BasicAttackScript;
	}
	break;
	case SkillType::JINX_BASIC_ATTACK:
	{
		CJinxBasicAttackScript* JinxBasicAttackScript = new CJinxBasicAttackScript;
		JinxBasicAttackScript->SetSkillType(SkillType::JINX_BASIC_ATTACK);
		return JinxBasicAttackScript;
	}
	break;
	case SkillType::JINX_ROCKET_BASIC_ATTACK:
	{		
		CJinxRocketBasicAttackScript* JinxRocketBasicAttackScript = new CJinxRocketBasicAttackScript;
		JinxRocketBasicAttackScript->SetSkillType(SkillType::JINX_ROCKET_BASIC_ATTACK);
		return JinxRocketBasicAttackScript;
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


