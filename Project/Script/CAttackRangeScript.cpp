#include "pch.h"
#include "CAttackRangeScript.h"
#include "CUnitScript.h"



CAttackRangeScript::CAttackRangeScript()
	: CScript((UINT)SCRIPT_TYPE::ATTACKRANGESCRIPT)
	, m_vecUnitsInRange{}
{
}

CAttackRangeScript::~CAttackRangeScript()
{
}

void CAttackRangeScript::begin()
{
	GetOwner()->ChangeLayer(L"AttackRange");
}

void CAttackRangeScript::BeginOverlap(CCollider2D* _Other)
{
	// ����
	CUnitScript* User = GetOwner()->GetParent()->GetScript<CUnitScript>();

	// Ÿ�� ����
	CUnitScript* Unit = _Other->GetOwner()->GetScript<CUnitScript>();
	if (Unit == nullptr)
		return;

	// è�Ǿ� / ��ž�� �ݴ� ����, �߸� ���� ���� ����
	if (User->GetUnitType() == UnitType::CHAMPION || User->GetUnitType() == UnitType::TURRET)
	{
		// ���ΰ� Ÿ�� ������ ������ �ٸ��ٸ�
		if (User->GetFaction() != Unit->GetFaction())
		{
			// ��Ÿ� ���� ������Ʈ�� ���
			m_vecUnitsInRange.push_back(_Other->GetOwner());
		}
	}
	else
	{
		// �̴Ͼ�, ���۸��� ��� �ٸ� �߸� ������ �������� ����
		if (User->GetFaction() != Unit->GetFaction() && Unit->GetFaction() != Faction::NONE)
		{
			// ��Ÿ� ���� ������Ʈ�� ���
			m_vecUnitsInRange.push_back(_Other->GetOwner());
		}
	}
}

void CAttackRangeScript::EndOverlap(CCollider2D* _Other)
{
	for (auto it = m_vecUnitsInRange.begin(); it != m_vecUnitsInRange.end(); )
	{
		if (*it == _Other->GetOwner())
		{
			it = m_vecUnitsInRange.erase(it);
		}
		else
			++it;
	}
}
