#include "pch.h"
#include "CMalphiteBasicAttackScript.h"

#include <Engine/CKeyMgr.h>

CMalphiteBasicAttackScript::CMalphiteBasicAttackScript()
	: CScript((UINT)SCRIPT_TYPE::MALPHITEBASICATTACKSCRIPT)
	, m_bStartingPoint(false)
{
}

CMalphiteBasicAttackScript::~CMalphiteBasicAttackScript()
{
}

void CMalphiteBasicAttackScript::tick()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::X) == KEY_STATE::TAP)
	{
		m_bStartingPoint = true;
		GetOwner()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	}

	if (m_bStartingPoint == true)
	{
		Vec3 Scale = GetOwner()->Transform()->GetRelativeScale();

		if (Scale.x > 100.f)
			DestroyObject(GetOwner());


		float NextScale = Scale.x + DT * 800.f;

		GetOwner()->Transform()->SetRelativeScale(NextScale, NextScale, 0.f);


	}
}


