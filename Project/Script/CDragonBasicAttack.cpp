#include "pch.h"
#include "CDragonBasicAttack.h"

CDragonBasicAttack::CDragonBasicAttack()
{
	m_strSkillName = L"DragonBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	// ����ü ����
	CGameObject* DragonBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\DragonBreathEffect.prefab")->Instantiate();
	DragonBasicAttackObj->Transform()->SetRelativeScale(100.f, 150.f, 1.f);
	//DragonBasicAttackObj->Animator2D()->Play(L"DragonBasicAttack", true);
	DragonBasicAttackObj->SetName(L"DragonBasicAttackProjectile");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = DragonBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// �ǰ� ����Ʈ
	Ptr<CPrefab> DragonBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteBasicAttackHitEffect.prefab");
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
