#include "pch.h"
#include "CIconTimerUIScript.h"

CIconTimerUIScript::CIconTimerUIScript()
	:CScript((UINT)SCRIPT_TYPE::ICONTIMERUISCRIPT)
	, m_fCurTimer(0.f)
	, m_fAlpha(0.f)
	, m_fTimer(0.f)
	, m_fDuration(0.8f)
{
}

CIconTimerUIScript::~CIconTimerUIScript()
{
}



void CIconTimerUIScript::begin()
{
}

void CIconTimerUIScript::tick()
{
	m_fTimer += DT; // 델타 타임을 누적하여 경과 시간을 계산

	if (m_fTimer <= m_fDuration) {
		float progress = m_fTimer / m_fDuration; // 경과 시간을 기반으로 0~1 사이의 진행도를 계산
		float easeOutProgress = 1 - pow(1 - progress, 2); // ease-out 효과 적용
		m_fAlpha = easeOutProgress; // 계산된 보간 값을 알파 값에 적용
	}
	else if (m_fTimer >= 2.3f && m_fTimer <= 2.8f) {
		float progress = (m_fTimer - 2.3f) / 0.5f; // 2.3f부터 2.8f 사이의 진행도를 계산
		float easeInProgress = pow(progress, 2); // ease-in 효과 적용
		m_fAlpha = 1.0f - easeInProgress; // 계산된 보간 값을 알파 값에 적용
	}
	else if (m_fTimer > 2.8f)
	{
		m_fAlpha = 0.f; // 최종 알파 값 설정
	}

	int i = 1;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_0, &i);
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fAlpha);
}

