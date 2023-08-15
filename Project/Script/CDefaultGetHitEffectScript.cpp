#include "pch.h"
#include "CDefaultGetHitEffectScript.h"

CDefaultGetHitEffectScript::CDefaultGetHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::DEFAULTGETHITEFFECTSCRIPT)
	, m_bUseBillBoard(false)
	, m_fDeleteScale(30.f)
	, m_fExpandSpeed(650.f)
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
	Vec3 Scale = GetOwner()->Transform()->GetRelativeScale();

	if (Scale.x > m_fDeleteScale)
		DestroyObject(GetOwner());

	float NextScale = Scale.x + DT * m_fExpandSpeed;

	GetOwner()->Transform()->SetRelativeScale(NextScale, NextScale, 1.f);
}