#include "pch.h"
#include "CJinxQ.h"

#include "CJinxScript.h"

CJinxQ::CJinxQ()
	: m_eWeaponMode(JinxWeaponMode::MINIGUN)
	, m_fSkillDamage(0)
	, m_fSkillAttackSpeed(0)
	, m_fSkillAttackRange(0)
{
	m_strSkillName = L"Switchheroo!";
	m_fCoolDown = 2.f;
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
	if (m_eWeaponMode == JinxWeaponMode::MINIGUN)
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
	if (m_eWeaponMode == JinxWeaponMode::MINIGUN)
		m_eWeaponMode = JinxWeaponMode::ROCKET_LAUNCHER;
	else
		m_eWeaponMode = JinxWeaponMode::MINIGUN;

	// ¡ũ�� ��ũ��Ʈ�� ���� ��� ����
	CJinxScript* Jinx = dynamic_cast<CJinxScript*>(m_OwnerScript);
	if (Jinx != nullptr)
	{
		Jinx->SetWeaponMode(m_eWeaponMode);
	}

	// ��Ÿ�� �ʱ�ȭ
	m_fCurCoolDown = m_fCoolDown;

	return true;
}
