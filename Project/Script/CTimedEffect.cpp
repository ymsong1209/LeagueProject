#include "pch.h"
#include "CTimedEffect.h"
#include <Engine\CTimeMgr.h>

#include "CUnitScript.h"

CTimedEffect::CTimedEffect(CUnitScript* targetScript, float duration, float damagePerTick, int numTicks, CC ccType)
	: m_TargetScript(targetScript)
	, m_fDuration(duration)
	, m_fDamagePerTick(damagePerTick)
	, m_iNumTicks(numTicks)
	, m_eCCType(ccType)
	, m_fTimeSinceLastTick(0.f)
{
}

CTimedEffect::~CTimedEffect()
{
}

void CTimedEffect::tick()
{
	m_fDuration -= DT;
	m_fTimeSinceLastTick += DT;

	if (m_fTimeSinceLastTick >= m_fDuration / m_iNumTicks)
	{
		m_fTimeSinceLastTick -= m_fDuration / m_iNumTicks;
		applyDamage();
	}
	
}

void CTimedEffect::applyCC()
{

}

void CTimedEffect::applyDamage()
{
	m_TargetScript->SetCurHPVar(-m_fDamagePerTick);
}
