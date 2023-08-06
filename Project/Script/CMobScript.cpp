#include "pch.h"
#include "CMobScript.h"
#include "CBasicAttack.h"

CMobScript::CMobScript(UINT ScriptType)
	:CUnitScript(ScriptType)
{
}

CMobScript::CMobScript()
	:CUnitScript((UINT)SCRIPT_TYPE::MOBSCRIPT)
{
}

CMobScript::~CMobScript()
{
}


void CMobScript::begin()
{
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	//m_Skill[0]->SetProjectileObj(); // 투사체 프리팹 설정
	m_SkillLevel[0] = 1;

	m_eCurCC = CC::CLEAR;
	m_eRestraint = RESTRAINT::DEFAULT;

	CUnitScript::begin();
}

void CMobScript::tick()
{
	CUnitScript::tick();
}

void CMobScript::SaveToLevelFile(FILE* _File)
{
	CUnitScript::SaveToLevelFile(_File);
}

void CMobScript::LoadFromLevelFile(FILE* _FILE)
{
	CUnitScript::LoadFromLevelFile(_FILE);
}

void CMobScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CUnitScript::SaveToLevelJsonFile(_objValue, allocator);
}

void CMobScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CUnitScript::LoadFromLevelJsonFile(_componentValue);
}
