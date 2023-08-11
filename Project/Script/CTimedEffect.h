#pragma once

class CUnitScript;

class CTimedEffect
{
public:
	CTimedEffect(CUnitScript* targetScript, float duration, float damagePerTick, int numTicks, CC ccType = CC::NO_CC);
	~CTimedEffect();

private:
	CUnitScript*	m_TargetScript;
	float			m_fDuration;				// ���ӽð�
	float			m_fDamagePerTick;			// ƽ �� ������
	int				m_iNumTicks;				// ���ӽð��� �� ƽ���� ������
	CC				m_eCCType;					// �ݿ��� CC��
	float			m_fTimeSinceLastTick;		// ƽ ���� �ð�

public:
	void tick();

	bool isFinished() const { return m_fDuration <= 0.f; }

	void applyCC(CC _ccType);
	void applyDamage();
};


