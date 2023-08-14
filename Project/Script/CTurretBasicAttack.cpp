#include "pch.h"
#include "CTurretBasicAttack.h"

CTurretBasicAttack::CTurretBasicAttack()
{
	m_strSkillName = L"TurretBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	//// ����ü
	//CGameObject* Projectile = new CGameObject;
	//Projectile->AddComponent(new CTransform);
	//Projectile->AddComponent(new CCollider2D);
	//Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	//Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 5.f));
	//Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	//Projectile->SetName(L"TurretProjectile");
	
	// ����Ʈ�� �׳� ����ü�� ����(������)
	Ptr<CPrefab> TurretBlaze = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\TurretBlaze.prefab");
	CGameObject* TurretBlazeObject = TurretBlaze->Instantiate();
	TurretBlazeObject->Transform()->SetRelativeScale(50, 50, 1); // ���� �۰� ������ ���� //  blaze�� ������ ���� ũ��
	TurretBlazeObject->AddComponent(new CCollider2D);
	TurretBlazeObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	TurretBlazeObject->Collider2D()->SetOffsetScale(Vec2(1.f/10, 1.f/10)); // ���� �۰� ����)	 // �浹 ũ��� trasnform�� scale�� �⺻�� �Ȱ��Ƽ� offset���� ���ϴ� ��ŭ ����
	TurretBlazeObject->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	TurretBlazeObject->SetName(L"TurretProjectile");

	// ���� ����ü�� ���������� ����
	Ptr<CPrefab> NewPrefab = new CPrefab;
	//CGameObject* PrefabObject = Projectile->Clone();
	NewPrefab->RegisterProtoObject(TurretBlazeObject);
	
	m_vecSkillObj.push_back(NewPrefab);

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;

	// �� ��ų�� �ǰ� ����Ʈ ��� (�ӽ÷� ���� �ǰ�)
	Ptr<CPrefab> MalphiteBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteBasicAttackHitEffect.prefab");
	m_SkillHitEffect = MalphiteBasicAttackHitEffect;
}

CTurretBasicAttack::~CTurretBasicAttack()
{
}

void CTurretBasicAttack::tick()
{
}

bool CTurretBasicAttack::Use()
{
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
		SkillType::TURRET_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

	return true;
}

// ���� �÷��̾ �� �Լ��� ���� ����� ��.
void CTurretBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	// ��Ÿ ����ü�� ���� �浹�� �� �Լ��� ���� ��ũ��Ʈ �־ ȣ���� ����

	float Damage = 0;

	// �������� ����, �⺻ ���ݷ� � ���� ������ ���
	CUnitScript* UnitScript = dynamic_cast<CUnitScript*>(_UserScript);
	if (UnitScript != nullptr)
	{
		float BaseDamage = 0.f;
		float AttackPow = UnitScript->GetAttackPower();

		// �����Դϴ�
		Damage = AttackPow;
	}

	// ���������� Ÿ���� ���¸�ŭ�� ���� �� ���� �ݿ��� ������ ���
	float DefencePow = _TargetScript->GetDefencePower();

	Damage -= DefencePow;

	float minDam = 10.f;
	if (Damage < minDam)
	{
		// ������ �ּҰ�
		Damage = minDam;
	}

	_TargetScript->SetCurHPVar(-Damage);


	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);

	// ���� �� �ǰ� ����Ʈ ��Ŷ ����
	//CSendServerEventMgr::GetInst()->��¼�� (����, Ÿ��, ��ųŸ�� ��)

	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());

	
}
