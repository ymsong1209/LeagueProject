#include "pch.h"
#include "CMalphiteE.h"

#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CMalphiteEScript.h"
#include "CTimedEffect.h"

CMalphiteE::CMalphiteE()
{
	m_strSkillName = L"GroundSlam";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;

	CGameObject* MalphiteEDecal = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteEDecal.prefab")->Instantiate();
	MalphiteEDecal->AddComponent(new CCollider2D);
	MalphiteEDecal->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	MalphiteEDecal->Collider2D()->SetOffsetScale(Vec2(80.f, 80.f));
	MalphiteEDecal->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	MalphiteEDecal->Collider2D()->SetDrawCollision(true);
	MalphiteEDecal->SetName(L"GroundSlam");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = MalphiteEDecal->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	//Ptr<CPrefab> MalphiteEEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteEDecal.prefab");
	//m_SkillHitEffect = MalphiteEEffect;

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;
	m_ProjectileScript = new CMalphiteEScript;
}

CMalphiteE::~CMalphiteE()
{
}

void CMalphiteE::tick()
{
	CSkill::tick();
}

bool CMalphiteE::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// ��Ÿ������ ��� UINT64_MAX�� ���ּ���
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(3),
		SkillType::MALPHITE_E,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0.f,0.f,0.f));

	// ��Ÿ�� �ʱ�ȭ
	m_fCurCoolDown = m_fCoolDown;

	return true;
}

void CMalphiteE::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
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
	CTimedEffect* MalphiteESlow = new CTimedEffect(TargetUnitScript, 2.f, 0, 0, CC::SLOW);
	TargetUnitScript->AddTimedEffect(MalphiteESlow);


	CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);

	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
