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
	Safe_Del_Vec(m_vecUnitsInRange);
}


void CAttackRangeScript::BeginOverlap(CCollider2D* _Other)
{
	// 본인
	CUnitScript* User = GetOwner()->GetParent()->GetScript<CUnitScript>();

	// 타겟 유닛
	CUnitScript* Unit = _Other->GetOwner()->GetScript<CUnitScript>();
	if (Unit == nullptr)
		return;

	// 본인과 타겟 유닛의 진영이 다르다면
	if (User->GetFaction() != Unit->GetFaction())
	{
		// 사거리 내의 오브젝트로 등록
		m_vecUnitsInRange.push_back(_Other->GetOwner());
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
