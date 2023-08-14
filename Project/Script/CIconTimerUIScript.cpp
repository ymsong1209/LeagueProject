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
	m_fTimer += DT; // ��Ÿ Ÿ���� �����Ͽ� ��� �ð��� ���

	if (m_fTimer <= m_fDuration) {
		float progress = m_fTimer / m_fDuration; // ��� �ð��� ������� 0~1 ������ ���൵�� ���
		float easeOutProgress = 1 - pow(1 - progress, 2); // ease-out ȿ�� ����
		m_fAlpha = easeOutProgress; // ���� ���� ���� ���� ���� ����
	}
	else if (m_fTimer >= 2.3f && m_fTimer <= 2.8f) {
		float progress = (m_fTimer - 2.3f) / 0.5f; // 2.3f���� 2.8f ������ ���൵�� ���
		float easeInProgress = pow(progress, 2); // ease-in ȿ�� ����
		m_fAlpha = 1.0f - easeInProgress; // ���� ���� ���� ���� ���� ����
	}
	else if (m_fTimer > 2.8f)
	{
		m_fAlpha = 0.f; // ���� ���� �� ����
	}

	int i = 1;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_0, &i);
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fAlpha);
}

