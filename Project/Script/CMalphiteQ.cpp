#include "pch.h"
#include "CMalphiteQ.h"

#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CMalphiteRScript.h"
#include "CTimedEffect.h"

CMalphiteQ::CMalphiteQ()
{
	m_strSkillName = L"UnstoppableForce";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;

	CGameObject* Projectile = new CGameObject;
	Projectile->AddComponent(new CTransform);
	Projectile->AddComponent(new CCollider2D);
	Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
	Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 20.f));
	Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	Projectile->Collider2D()->SetDrawCollision(true);
	Projectile->SetName(L"UnstoppableForce");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = Projectile->Clone();
	NewPrefab->RegisterProtoObject(Projectile);

	m_vecSkillObj.push_back(NewPrefab);

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;
	m_ProjectileScript = new CMalphiteRScript;
}

CMalphiteQ::~CMalphiteQ()
{
}

void CMalphiteQ::tick()
{
	CSkill::tick();
}

bool CMalphiteQ::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// ��Ÿ������ ��� UINT64_MAX�� ���ּ���
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(2),
		SkillType::JINX_W,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		true,
		GetMouseDir());

	// ��Ÿ�� �ʱ�ȭ
	m_fCurCoolDown = m_fCoolDown;

	return true;
}

void CMalphiteQ::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// ¡ũ�� w ����ü�� ���� �浹�� �� �Լ��� ���� ��ũ��Ʈ �־ ȣ���� ����

	float Damage = 0;

	// �������� ����, �⺻ ���ݷ� � ���� ������ ���
	CChampionScript* ChamScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChamScript != nullptr)
	{
		float BaseDamage = 50.f;
		int   level = ChamScript->GetLevel();
		float AttackPow = ChamScript->GetAttackPower();

		// �����Դϴ�
		Damage = BaseDamage + (level * 2) + (AttackPow * 0.3f);
	}

	CUnitScript* TargetUnitScript = dynamic_cast<CUnitScript*>(_TargetScript);
	if (TargetUnitScript != nullptr)
	{
		float DefencePow = TargetUnitScript->GetDefencePower();

		Damage -= DefencePow;

		float minDam = 20.f;

		if (Damage < minDam)
		{
			// ������ �ּҰ�
			Damage = minDam;
		}
	}

	TargetUnitScript->SetCurHPVar(-Damage);

	// 2�� ���� ��ȭ��ŵ�ϴ�.
	CTimedEffect* JinxWSlow = new CTimedEffect(TargetUnitScript, 2.f, 0, 0, CC::SLOW);
	TargetUnitScript->AddTimedEffect(JinxWSlow);

	// �׽�Ʈ�� ��Ʈ��
	CTimedEffect* TestDot = new CTimedEffect(TargetUnitScript, 3.f, 5.f, 6, CC::CLEAR);
	TargetUnitScript->AddTimedEffect(TestDot);

	CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);

	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}