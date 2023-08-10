#include "pch.h"
#include "CGrowScaleScript.h"
#include <Engine/CKeyMgr.h>

CGrowScaleScript::CGrowScaleScript()
	: CScript((UINT)SCRIPT_TYPE::GROWSCALESCRIPT)
{
}

CGrowScaleScript::~CGrowScaleScript()
{
}

void CGrowScaleScript::begin()
{
}

void CGrowScaleScript::tick()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::X) == KEY_STATE::TAP)
	{

	}
}


