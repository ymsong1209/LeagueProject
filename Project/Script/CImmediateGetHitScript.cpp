#include "pch.h"
#include "CImmediateGetHitScript.h"

CImmediateGetHitScript::CImmediateGetHitScript()
	: CScript((UINT)SCRIPT_TYPE::IMMEDIATEGETHITSCRIPT)
	, m_fCurTime(0.f)
	, m_fTriggerTime(0.f)
{
}

CImmediateGetHitScript::~CImmediateGetHitScript()
{
}

void CImmediateGetHitScript::begin()
{
	int a = 0;

	m_fCurTime = 0.f;




	 
}

void CImmediateGetHitScript::tick()
{
	m_fCurTime += DT;

	if (m_fCurTime >= m_fTriggerTime)
	{
		DestroyObject(GetOwner());
	}

	int b = 0;
}
