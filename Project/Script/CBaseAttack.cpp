#include "pch.h"
#include "CBaseAttack.h"
#include <Engine/CPrefab.h>
#include <Engine/ptr.h>
#include "CUnitScript.h"
#include "CChampionScript.h"

CBaseAttack::CBaseAttack()
{
	m_strSkillName = L"BaseAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;
}

CBaseAttack::~CBaseAttack()
{
}

void CBaseAttack::tick()
{
	// 쿨타임 계산 필요 없음
}

bool CBaseAttack::Use()
{
	// 평타 투사체 생성

	if (m_vecSkillObj.size() == 0)
		return false;

	m_vecSkillObj[0]->Instantiate();

	return true;
}

void CBaseAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript)
{
	// 평타 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임

	float Damage = 0;

	// 시전자의 레벨, 기본 공격력 등에 따라 데미지 계산
	CChampionScript* ChampScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChampScript != nullptr)
	{
		float BaseDamage = 0.f;
		float AttackPow = ChampScript->GetAttackPower();

		// 예시입니다
		Damage = AttackPow;
	}

	// 데미지에서 타겟의 방어력만큼을 제한 뒤 실제 반영할 데미지 계산
	float DefencePow = _TargetScript->GetDefencePower();
	
	Damage -= DefencePow;

	float minDam = 10.f;
	if (Damage < minDam)
	{
		// 데미지 최소값
		Damage = minDam;
	}

	_TargetScript->SetCurHPVar(-Damage);
}
