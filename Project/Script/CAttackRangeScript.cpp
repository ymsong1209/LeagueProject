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
	// 본인
	CUnitScript* User = GetOwner()->GetParent()->GetScript<CUnitScript>();

	// 타겟 유닛
	CUnitScript* Unit = _Other->GetOwner()->GetScript<CUnitScript>();
	if (Unit == nullptr)
		return;

	// 챔피언 / 포탑은 반대 진영, 중립 몬스터 공격 가능
	if (User->GetUnitType() == UnitType::CHAMPION || User->GetUnitType() == UnitType::TURRET)
	{
		// 본인과 타겟 유닛의 진영이 다르다면
		if (User->GetFaction() != Unit->GetFaction())
		{
			// 사거리 내의 오브젝트로 등록
			m_vecUnitsInRange.push_back(_Other->GetOwner());
		}
	}
	else
	{
		// 미니언, 정글몹의 경우 다른 중립 진영은 공격하지 않음
		if (User->GetFaction() != Unit->GetFaction() && Unit->GetFaction() != Faction::NONE)
		{
			// 사거리 내의 오브젝트로 등록
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
