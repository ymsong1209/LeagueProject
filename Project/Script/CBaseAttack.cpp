#include "pch.h"
#include "CBaseAttack.h"
#include <Engine/CPrefab.h>
#include <Engine/ptr.h>
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CBasicAttackScript.h"

CBaseAttack::CBaseAttack()
{
	m_strSkillName = L"BaseAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	// 투사체 생성
	CGameObject* Projectile = new CGameObject;
	Projectile->AddComponent(new CTransform);
	Projectile->AddComponent(new CCollider2D);
	Projectile->AddComponent(new CBasicAttackScript);
	Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 5.f));
	Projectile->Collider2D()->SetOffsetRot(Vec3(90.f, 0.f, 0.f));
	Projectile->SetName(L"Projectile");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = Projectile->Clone(); 
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back( NewPrefab);
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

	CGameObject* bullet = m_vecSkillObj[0]->Instantiate();
	Vec3 OwnerPos = m_OwnerScript->GetOwner()->Transform()->GetRelativePos();

	CBasicAttackScript* AttackScript = bullet->GetScript<CBasicAttackScript>();
	AttackScript->SetTargetID(m_iTargetID);
	AttackScript->SetUserID(m_iUserID);

	SpawnGameObject(bullet, OwnerPos, 0);

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
