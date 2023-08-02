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
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// W ����ü �߻�
	/*
	¡ũ�� w�� ���÷� ���
		1) ����ǥ�� ����Ʈ�� ��Ÿ���� �����(2d �ִϸ��̼�)*/

	
		//2) ����Ʈ�� �������, ���� �浹ü�� ���� ������Ʈ �߻� 
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
			2-1) ������Ʈ�� �ڽ����� �׸��� ����Ʈ, �ֺ� ��ƼŬ, ���� ���� ���� (�θ�-�ڽ� ����)

			3) ������Ʈ�� Ư�� �Ÿ����� �߻�ǰų� (�ƹ��� �ȸ´� ���)
			�������� �°� �Ǹ� �����. (�� ��쿡�� �ǰ�)
			*/
	
}

void CJinxW::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// ¡ũ�� w ����ü�� ���� �浹�� �� �Լ��� ���� ��ũ��Ʈ �־ ȣ���� ����

	float Damage;

	// �������� ����, �⺻ ���ݷ� � ���� ������ ���
	CChampionScript* ChampScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChampScript != nullptr)
	{
		float BaseDamage = 50.f;
		int   level		 = ChampScript->GetLevel();
		float AttackPow  = ChampScript->GetAttackPower();

		// �����Դϴ�
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
			// ������ �ּҰ�
			Damage = minDam;
		}
	}

	TargetChampScript->SetCurHPVar(-Damage);

	// 2�� ���� ��ȭ��Ű�� ��ġ�� �巯���ϴ�.

}
