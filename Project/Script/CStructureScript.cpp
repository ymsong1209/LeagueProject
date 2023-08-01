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
	// ü��
	m_fHP = m_fMaxHP;
}

void CStructureScript::tick()
{
	if (!m_bAttackable)
	{
		// ���� �Ұ� �� ���� �Ұ�
	}
}
