#include "pch.h"
#include "CMobScript.h"

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
	CUnitScript::begin();
}

void CMobScript::tick()
{
}
