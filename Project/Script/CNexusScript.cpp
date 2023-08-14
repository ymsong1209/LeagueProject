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
	// 4번 포탑이 전부 Dead 상태여야 Attack 가능
}

