#include "pch.h"
#include "CMalphiteBasicAttackHitEffectScript.h"

#include <Engine/CKeyMgr.h>

CMalphiteBasicAttackHitEffectScript::CMalphiteBasicAttackHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::MALPHITEBASICATTACKHITEFFECTSCRIPT)
	, m_bStartingPoint(false)
{
}

CMalphiteBasicAttackHitEffectScript::~CMalphiteBasicAttackHitEffectScript()
{
}

void CMalphiteBasicAttackHitEffectScript::tick()
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

 

