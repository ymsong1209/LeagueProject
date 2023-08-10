#include "pch.h"
#include "CStructureScript.h"

CStructureScript::CStructureScript(UINT ScriptType)
	:CUnitScript(ScriptType)
{
}

CStructureScript::CStructureScript()
	:CUnitScript((UINT)SCRIPT_TYPE::STRUCTURESCRIPT)
{
}

CStructureScript::~CStructureScript()
{
}

void CStructureScript::begin()
{
	// 체력
	m_fHP = m_fMaxHP;

	// 외곽선 기능
	GetOwner()->Transform()->SetIsShootingRay(true);
}

void CStructureScript::tick()
{
	if (!m_bAttackable)
	{
		// 선택 불가 및 공격 불가
	}
}
