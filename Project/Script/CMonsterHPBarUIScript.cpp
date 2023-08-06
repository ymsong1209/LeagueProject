#include "pch.h"
#include "CMonsterHPBarUIScript.h"
#include "CUnitScript.h"

void CMonsterHPBarUIScript::begin()
{
	//GetOwner()->GetScript<CUnitScript>()->GetCurHP();
	//GetOwner()->GetScript<CUnitScript>()->GetMaxHP
}

void CMonsterHPBarUIScript::tick()
{
}

void CMonsterHPBarUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CMonsterHPBarUIScript::CMonsterHPBarUIScript()
	:CScript((UINT)SCRIPT_TYPE::MONSTERHPBARUISCRIPT)
{
}

CMonsterHPBarUIScript::~CMonsterHPBarUIScript()
{
}
