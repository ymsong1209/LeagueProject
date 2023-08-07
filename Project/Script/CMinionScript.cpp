#include "pch.h"
#include "CMinionScript.h"

CMinionScript::CMinionScript()
	:CMobScript((UINT)SCRIPT_TYPE::MINIONSCRIPT)
	, m_eLane(Lane::NONE)
	, m_vecTargetPoint{}
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_eMinionType, "Type");
}

CMinionScript::~CMinionScript()
{
}

void CMinionScript::begin()
{

	GetOwner()->Transform()->SetUseMouseOutline(true);
	// ����
	SetFaction(Faction::RED);

	// ������ Lane, MinionType�� ���� ���� ����

	switch (m_eLane)
	{
	case Lane::TOP:
	{
		// Top �������� �������� �� m_vecTargetPoint�� pushback
	}
		break;
	case Lane::MID:
	{
		// Mid �������� �������� �� m_vecTargetPoint�� pushback
	}
		break;
	case Lane::BOTTOM:
	{
		// Bot �������� �������� �� m_vecTargetPoint�� pushback
	}
		break;
	}

	switch (m_eMinionType)
	{
	case MinionType::MELEE:
	{
		m_fAttackPower = 1.f;
		m_fAttackRange = 50.f;
		m_fAttackSpeed = 1.f;
		m_fMaxHP = 50.f;
	}
		break;
	case MinionType::RANGED:
		break;
	case MinionType::SEIGE:
		break;
	case MinionType::SUPER:
		break;
	}

	m_fHP = m_fMaxHP;
}

void CMinionScript::tick() 
{
	/*
	1. ���κ� Ÿ�� ����Ʈ�� �����Ѵ�.
	2. Ÿ������Ʈ�� �����ϸ�, ������ �ε����� �������� ���� Ÿ������Ʈ�� �����Ѵ�.
	3. ���� ���� ��Ÿ� ���� ������ �� �ִ� ���� ������, ���� ���·� ��ȯ�Ѵ�.
	4. ��Ÿ� ���� �� �� �켱������ ���� Ÿ�� ������Ʈ�� �����Ѵ�.
	5. Ÿ�� ������Ʈ�� �� �̻� ��Ÿ� ���� ���ų� �׾��ٸ�, Ÿ�� ������Ʈ�� �ٽ� �����Ѵ�.
	6. �ٽ� ��Ÿ� ���� ������ ������Ʈ�� ���ٸ�, Walk ���·� ��ȯ�Ǿ� ������ ����Ѵ�.
	*/
}
