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
	if (m_eCCType != CC::NO_CC)
	{
		m_TargetScript->RemoveCC(m_eCCType);
	}
}

void CTimedEffect::tick()
{
	m_fDuration -= DT;

	if (m_fDamagePerTick != 0 || m_iNumTicks != 0)
	{
		m_fTimeSinceLastTick += DT;

		if (m_fTimeSinceLastTick >= m_fDuration / m_iNumTicks)
		{
			m_fTimeSinceLastTick -= m_fDuration / m_iNumTicks;
			applyDamage();
		}
	}

	if (m_eCCType != CC::NO_CC)
	{
		applyCC(m_eCCType);
	}
}

void CTimedEffect::applyCC(CC _ccType)
{
	m_TargetScript->ApplyCC(m_eCCType);
}

void CTimedEffect::applyDamage()
{
	m_TargetScript->SetCurHPVar(-m_fDamagePerTick);
}
