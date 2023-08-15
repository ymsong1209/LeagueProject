#include "pch.h"
#include "CRangedMinionBasicAttack.h"

CRangedMinionBasicAttack::CRangedMinionBasicAttack()
{
	m_strSkillName = L"RangedMinionBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	CGameObject* RangedMinionBasicAttackObj = nullptr;

	// ����ü ����
	if (m_OwnerScript->GetFaction() == Faction::RED)
	{
		CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RedMinionBasicAttack.prefab")->Instantiate();
		RangedMinionBasicAttackObj->SetName(L"RedRangedMinionBasicAttackProjectile");
		RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionRedAttack.dds"));
	
		// �ǰ� ����Ʈ
		Ptr<CPrefab> MinionBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MinionRedGetHitByBasicAttack.prefab");
		m_SkillHitEffect = MinionBasicAttackHitEffect;
	}
	else
	{
		CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BlueMinionBasicAttack.prefab")->Instantiate();
		RangedMinionBasicAttackObj->SetName(L"BlueRangedMinionBasicAttackProjectile");
		RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionBlueAttack.dds"));
	
		// �ǰ� ����Ʈ
		Ptr<CPrefab> MinionBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MinionBlueGetHitByBasicAttack.prefab");
		m_SkillHitEffect = MinionBasicAttackHitEffect;
	}

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = RangedMinionBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;
}

CRangedMinionBasicAttack::~CRangedMinionBasicAttack()
{

}

void CRangedMinionBasicAttack::tick()
{
	// ��Ÿ�� ��� �ʿ� ����
}

bool CRangedMinionBasicAttack::Use()
{
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
		SkillType::BASIC_RANGED_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

	return true;
}

void CRangedMinionBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _Skilllevel)
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
