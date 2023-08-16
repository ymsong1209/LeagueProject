#include "pch.h"
#include "CMinionBasicAttackHitEffectScript.h"

#include <Engine/CKeyMgr.h>

CMinionBasicAttackHitEffectScript::CMinionBasicAttackHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::MINIONBASICATTACKHITEFFECTSCRIPT)
	, m_bStartingPoint(false)
{
}

CMinionBasicAttackHitEffectScript::~CMinionBasicAttackHitEffectScript()
{
}

void CMinionBasicAttackHitEffectScript::begin()
{
	GetOwner()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	GetOwner()->Transform()->SetBillBoard(true);
}

void CMinionBasicAttackHitEffectScript::tick()
{
	if (CKeyMgr::GetInst()->GetInst()->GetKeyState(KEY::X) == KEY_STATE::TAP)
	{
		m_bStartingPoint = true;
	}

	if (m_bStartingPoint == true)
	{
		Vec3 Scale = GetOwner()->Transform()->GetRelativeScale();

		if (Scale.x > 30.f)
			DestroyObject(GetOwner());

		float NextScale = Scale.x + DT * 650.f;

		GetOwner()->Transform()->SetRelativeScale(NextScale, NextScale, 1.f);
	}
}

