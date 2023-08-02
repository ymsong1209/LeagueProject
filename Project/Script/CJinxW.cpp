#include "pch.h"
#include "CJinxW.h"
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CJinxWScript.h"

CJinxW::CJinxW()
{
	m_strSkillName = L"Zap!";
	m_fCoolDown = 0.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;
}

CJinxW::~CJinxW()
{
}

void CJinxW::tick()
{
	CSkill::tick();
}

bool CJinxW::Use()
{
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// W 투사체 발사
	/*
	징크스 w를 예시로 들면
		1) 범위표시 이펙트가 나타났다 사라짐(2d 애니메이션)*/

	
		//2) 이펙트가 사라지고, 실제 충돌체를 가진 오브젝트 발사 
			//CGameObject* Projectile = new CGameObject;
			//Projectile->AddComponent(new CTransform);
			//Projectile->AddComponent(new CCollider2D);
			//Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
			//Projectile->Collider2D()->SetOffsetScale(Vec2(20.f, 5.f));
			//Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
			//Projectile->SetName(L"Projectile");
			//
			//
			//Projectile->AddComponent(new CJinxWScript);
			//Vec3 OwnerPos = m_OwnerScript->GetOwner()->Transform()->GetRelativePos();
			//Projectile->GetScript<CJinxWScript>()->SetSpawnPos(OwnerPos);

			//Ptr<CPrefab> NewPrefab = new CPrefab;
			//CGameObject* PrefabObject = Projectile->Clone();
			//NewPrefab->RegisterProtoObject(Projectile);

			//m_vecSkillObj.push_back(NewPrefab);
			
			/*
			2-1) 오브젝트의 자식으로 그림자 이펙트, 주변 파티클, 꼬리 등이 따라감 (부모-자식 구조)

			3) 오브젝트가 특정 거리까지 발사되거나 (아무도 안맞는 경우)
			누군가가 맞게 되면 사라짐. (이 경우에는 피격)
			*/
	
}

void CJinxW::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// 징크스 w 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임

	float Damage;

	// 시전자의 레벨, 기본 공격력 등에 따라 데미지 계산
	CChampionScript* ChampScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChampScript != nullptr)
	{
		float BaseDamage = 50.f;
		int   level		 = ChampScript->GetLevel();
		float AttackPow  = ChampScript->GetAttackPower();

		// 예시입니다
		Damage = BaseDamage + (level * 2) + (AttackPow * 0.3f);
	}
	
	CChampionScript* TargetChampScript = dynamic_cast<CChampionScript*>(_TargetScript);
	if (TargetChampScript != nullptr)
	{
		float DefencePow = TargetChampScript->GetDefencePower();

		Damage -= DefencePow;

		float minDam = 20.f;

		if (Damage < minDam)
		{
			// 데미지 최소값
			Damage = minDam;
		}
	}

	TargetChampScript->SetCurHPVar(-Damage);

	// 2초 동안 둔화시키며 위치를 드러냅니다.

}
