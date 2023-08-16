#include "pch.h"
#include "CDefaultGetHitEffectScript.h"

CDefaultGetHitEffectScript::CDefaultGetHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::DEFAULTGETHITEFFECTSCRIPT)
	, m_bUseBillBoard(false)
	, m_fDeleteScale(10.f)
	, m_fExpandSpeed(400.f)
	, m_bIsmaxScale(false)
{
}

CDefaultGetHitEffectScript::~CDefaultGetHitEffectScript()
{
}

void CDefaultGetHitEffectScript::begin()
{
	GetOwner()->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));

	if (m_bUseBillBoard)
	{
		GetOwner()->Transform()->SetBillBoard(true);
	}	 
}

void CDefaultGetHitEffectScript::tick()
{
	if (m_bIsmaxScale) return;

	Vec3 Scale = GetOwner()->Transform()->GetRelativeScale();

	if (Scale.x > m_fDeleteScale)
	{
		m_bIsmaxScale = true;
		DestroyObject(GetOwner());
	}
		
	float NextScale = Scale.x + DT * m_fExpandSpeed;

	GetOwner()->Transform()->SetRelativeScale(NextScale, NextScale, 1.f);
}