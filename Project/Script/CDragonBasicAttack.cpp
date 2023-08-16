#include "pch.h"
#include "CDragonBasicAttack.h"
#include "CImmediateGetHitScript.h"

CDragonBasicAttack::CDragonBasicAttack()
{
	m_strSkillName = L"DragonBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	// ����ü ����
	CGameObject* DragonBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\DragonBreathEffect.prefab")->Instantiate();
	DragonBasicAttackObj->Transform()->SetRelativeScale(100.f, 150.f, 1.f);
	DragonBasicAttackObj->AddComponent(new CCollider2D);
	DragonBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	DragonBasicAttackObj->Collider2D()->SetAbsolute(true);
	DragonBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
	DragonBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	DragonBasicAttackObj->Collider2D()->SetDrawCollision(true);
	DragonBasicAttackObj->Transform()->SetBillBoard(false);
	DragonBasicAttackObj->SetName(L"DragonBasicAttackProjectile");
	//DragonBasicAttackObj->Transform()->SetBillBoard(true);
	//DragonBasicAttackObj->Animator2D()->Play(L"DragonBasicAttack", true);
		
	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = DragonBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// �ǰ� ����Ʈ
	Ptr<CPrefab> DragonBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteBasicAttackHitEffect.prefab");
	DragonBasicAttackHitEffect.Get()->GetProtoObject()->AddComponent(new CImmediateGetHitScript);
 

	m_SkillHitEffect = DragonBasicAttackHitEffect;

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;
}

CDragonBasicAttack::~CDragonBasicAttack()
{

}

void CDragonBasicAttack::tick()
{
	// ��Ÿ�� ��� �ʿ� ����
}

bool CDragonBasicAttack::Use()
{
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
		SkillType::DRAGON_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

	return true;
}

void CDragonBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _Skilllevel)
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


	CSkill::GetHit(_UserScript, _TargetScript, _Skilllevel);

	// ���� �� �ǰ� ����Ʈ ��Ŷ ����
	//CSendServerEventMgr::GetInst()->��¼�� (����, Ÿ��, ��ųŸ�� ��)

	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
