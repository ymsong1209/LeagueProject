#include "pch.h"
#include "CAnnounceTimerUIScript.h"
#include <Engine\CEventMgr.h>

void CAnnounceTimerUIScript::begin()
{
}

void CAnnounceTimerUIScript::tick()
{
	if(CEventMgr::GetInst()->GetIsGameOver())
		DestroyObject(GetOwner());

	m_fTimer += DT; // ��Ÿ Ÿ���� �����Ͽ� ��� �ð��� ���

	if (m_fTimer >= 2.9f && !GetOwner()->IsDead())
	{
		DestroyObject(GetOwner());
	}

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
	else if(m_fTimer > 2.8f)
	{
		m_fAlpha = 0.f; // ���� ���� �� ����
	}

	int i = 1;
	MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &i);
	MeshRender()->GetMaterial(0)->SetScalarParam(FLOAT_0, &m_fAlpha);
}

CAnnounceTimerUIScript::CAnnounceTimerUIScript()
	:CScript((UINT)SCRIPT_TYPE::ANNOUNCETIMERUISCRIPT)
	,m_fCurTimer(0.f)
	,m_fAlpha(0.f)
	,m_fTimer(0.f)
	,m_fDuration(0.8f)
{
}

CAnnounceTimerUIScript::~CAnnounceTimerUIScript()
{
}
