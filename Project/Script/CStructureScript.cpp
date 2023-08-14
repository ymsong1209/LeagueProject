#include "pch.h"
#include "CStructureScript.h"
#include <algorithm>

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
	// Ã¼·Â
	m_fHP = m_fMaxHP;
}

void CStructureScript::tick()
{
	CUnitScript::tick();
	//m_fHP = clamp(m_fHP, 0.f, m_fMaxHP);
}
