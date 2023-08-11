#pragma once

class CUnitScript;

class CTimedEffect
{
public:
	CTimedEffect(CUnitScript* targetScript, float duration, float damagePerTick, int numTicks, CC ccType = CC::NO_CC);
	~CTimedEffect();

private:
	CUnitScript*	m_TargetScript;
	float			m_fDuration;				// 지속시간
	float			m_fDamagePerTick;			// 틱 당 데미지
	int				m_iNumTicks;				// 지속시간을 몇 틱으로 나눌지
	CC				m_eCCType;					// 반영할 CC기
	float			m_fTimeSinceLastTick;		// 틱 계산용 시간

public:
	void tick();

	bool isFinished() const { return m_fDuration <= 0.f; }

	void applyCC(CC _ccType);
	void applyDamage();
};


