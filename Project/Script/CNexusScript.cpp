#include "pch.h"
#include "CNexusScript.h"

CNexusScript::CNexusScript()
	:CStructureScript((UINT)SCRIPT_TYPE::NEXUSSCRIPT)
{
}

CNexusScript::~CNexusScript()
{
}

void CNexusScript::begin()
{
	CStructureScript::begin();
}

void CNexusScript::tick()
{
	CStructureScript::tick();
}

void CNexusScript::CheckStatus()
{
	// 4�� ��ž�� ���� Dead ���¿��� Attack ����
}

