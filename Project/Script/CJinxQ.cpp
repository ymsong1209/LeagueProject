#include "pch.h"
#include "CJinxQ.h"

#include "CJinxScript.h"

CJinxQ::CJinxQ()
	: m_iWeaponMode(0)
	, m_fSkillDamage(0)
	, m_fSkillAttackSpeed(0)
	, m_fSkillAttackRange(0)
{
	m_strSkillName = L"Switchheroo!";
	m_fCoolDown = 0.9f;
	m_iMaxLevel = 5;
	m_fCost = 0.f;
}

CJinxQ::~CJinxQ()
{
}

void CJinxQ::tick()
{
	CSkill::tick();

	// ������ ���� ���ݷ� / ���ݼӵ� ����

	// �̴ϰ� ����� ��
	if (m_iWeaponMode == 0)
	{
		// ������
		m_fSkillDamage = m_iLevel + 5;
	}
	// ���� ��ó ����� ��
	else
	{
		// ������
		m_fSkillDamage = m_iLevel * 1.5 + 5;
		// ���ݼӵ�
		m_fSkillAttackSpeed = -20.f;
		// ��Ÿ�
		m_fSkillAttackRange = 5.f + (m_iLevel * 1.5f);
	}
}

bool CJinxQ::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// ��� �� ¡ũ���� ���⸦ �ٲ�
	m_iWeaponMode++;

	if (m_iWeaponMode > 1)
		m_iWeaponMode = 0;

	// ¡ũ�� ��ũ��Ʈ�� ���� ��� ����
	CJinxScript* Jinx = dynamic_cast<CJinxScript*>(m_OwnerScript);
	if (Jinx != nullptr)
	{
		Jinx->SetWeaponMode((JinxWeaponMode)m_iWeaponMode);
	}

	return true;
}
