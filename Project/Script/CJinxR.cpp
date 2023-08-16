#include "pch.h"
#include "CJinxR.h""
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CJinxRScript.h"
#include "CTimedEffect.h"
#include "CImmediateGetHitScript.h"

CJinxR::CJinxR()
{
	m_strSkillName = L"Super Mega Death Rocket!";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;

	CGameObject* pObj = nullptr;
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx_R.fbx");
	pObj = pMeshData->Instantiate();
	pObj->AddComponent(new CCollider2D);
	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	pObj->Collider2D()->SetDrawCollision(true);
	pObj->SetName(L"JinxR");
	pObj->Transform()->SetIsShootingRay(true);
	pObj->Transform()->SetRayRange(100);
	pObj->Transform()->SetRelativeScale(Vec3(0.7f, 0.7f, 0.7f));

	Ptr<CPrefab> NewPrefab = new CPrefab;
	NewPrefab->RegisterProtoObject(pObj);
	m_vecSkillObj.push_back(NewPrefab);

	// �ǰ� ����Ʈ
	CGameObject* JinxBasicAttackGetHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxGetHitByRocketAttack.prefab")->Instantiate();
	JinxBasicAttackGetHitEffect->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\jinxtex\\JinxGetHitByRocket.png"));
	JinxBasicAttackGetHitEffect->AddComponent(new CImmediateGetHitScript);
	JinxBasicAttackGetHitEffect->GetScript<CImmediateGetHitScript>()->SetTriggerTime(0.5f);
	Ptr<CPrefab> NewHitPrefab = new CPrefab;
	CGameObject* HitPrefabObject = JinxBasicAttackGetHitEffect->Clone();
	NewHitPrefab->RegisterProtoObject(HitPrefabObject);

	m_SkillHitEffect = NewHitPrefab;

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;
	m_ProjectileScript = new CJinxRScript;
}

CJinxR::~CJinxR()
{
}

void CJinxR::tick()
{
	CSkill::tick();
}

bool CJinxR::Use()
{
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// �������� �� ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// ��Ÿ������ ��� UINT64_MAX�� ���ּ���
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(4),
		SkillType::JINX_R,
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

void CJinxR::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// ¡ũ�� R ����ü�� ���� �浹�� �� �Լ��� ���� ��ũ��Ʈ �־ ȣ���� ����

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

	CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);

	// �ǰ��� ����� KDACS ��Ŷ ����
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
