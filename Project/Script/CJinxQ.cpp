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

	// 레벨에 따른 공격력 / 공격속도 증감

	// 미니건 모드일 때
	if (m_iWeaponMode == 0)
	{
		// 데미지
		m_fSkillDamage = m_iLevel + 5;
	}
	// 로켓 런처 모드일 때
	else
	{
		// 데미지
		m_fSkillDamage = m_iLevel * 1.5 + 5;
		// 공격속도
		m_fSkillAttackSpeed = -20.f;
		// 사거리
		m_fSkillAttackRange = 5.f + (m_iLevel * 1.5f);
	}
}

bool CJinxQ::Use()
{
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// 사용 시 징크스가 무기를 바꿈
	m_iWeaponMode++;

	if (m_iWeaponMode > 1)
		m_iWeaponMode = 0;

	// 징크스 스크립트의 무기 모드 변경
	CJinxScript* Jinx = dynamic_cast<CJinxScript*>(m_OwnerScript);
	if (Jinx != nullptr)
	{
		Jinx->SetWeaponMode((JinxWeaponMode)m_iWeaponMode);
	}

	return true;
}
