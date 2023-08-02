#include "pch.h"
#include "CSkill.h"
#include <Engine\CTimeMgr.h>

CSkill::CSkill()
	: m_strSkillName{}
	, m_fCurCoolDown(0.0f)
	, m_iLevel(1)
	, m_OwnerScript(nullptr)
	, m_vecSkillObj{}
{
}

CSkill::~CSkill()
{
}

void CSkill::tick()
{
	if (m_fCurCoolDown > 0.0f)
	{
		if (m_fCurCoolDown <= 0.0f)
			m_fCurCoolDown = 0.0f;
		else
			m_fCurCoolDown -= DT;
	}
	// �θ��� CSkill�� tick�� �⺻������ �� ������ ��Ÿ���� ������ֱ� ������ 
	// �ڽ� Ŭ�������� �� ���� ȣ��������մϴ�.
	// �� �ܿ�, �⺻ ����ȿ��(��� �ð� ���� ���ݷ� ��� ��)�� �ִ� ��ų�� ���
	// ���� �Լ��� tick�� �����ּ���.
}

bool CSkill::Use()
{
	// ��ų ������ 0�̰ų� ��Ÿ���� �� ���� ���� ��� ��� �Ұ�
	if (m_iLevel == 0 || m_fCurCoolDown > 0.0f)
	{
		return false;
	}
	else
	{
		// ��Ÿ�� �ʱ�ȭ
		m_fCurCoolDown = m_fCoolDown;

		return true;
	}

	// �θ��� CSkill�� Use�� ����� ������ ��Ÿ���� �ʱ�ȭ���ְ� 
	// Skill ������ 0�� ���� ��ų ��Ÿ���� �� ���� �ʾ��� ���� ��ų�� ����� �� ���� �մϴ�.
	// ���� �� �ڽ� Ŭ�������� ���� ȣ��������մϴ�!
	// (��ų ��� �Ұ� ���� / ���� ���� ���� ���� ChampionScript���� ����ó�� ���ݴϴ�.)
	// ���⼭ �� ��ų�� ��� ȿ��(ex. ����ü ����)�� �����մϴ�.


}
