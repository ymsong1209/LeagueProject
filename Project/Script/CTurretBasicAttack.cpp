#include "pch.h"
#include "CTurretBasicAttack.h"

CTurretBasicAttack::CTurretBasicAttack()
{
	m_strSkillName = L"TurretBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	// 투사체
	//CGameObject* Projectile = new CGameObject;
	//Projectile->AddComponent(new CTransform);
	//Projectile->AddComponent(new CCollider2D);
	//Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	//Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 5.f));
	//Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	//Projectile->SetName(L"Projectile");
	//
	//Ptr<CPrefab> NewPrefab = new CPrefab;
	//CGameObject* PrefabObject = Projectile->Clone();
	//NewPrefab->RegisterProtoObject(PrefabObject);

	Ptr<CPrefab> TurretBlaze = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\TurretBlaze.prefab");
	m_vecSkillObj.push_back(TurretBlaze);

	// 투사체 스크립트
	m_iProjectileCount = 1;

	// 피격 이펙트
	//Ptr<CPrefab> MalphiteBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteBasicAttackHitEffect.prefab");
	//m_SkillHitEffect = MalphiteBasicAttackHitEffect;
}

CTurretBasicAttack::~CTurretBasicAttack()
{
}

void CTurretBasicAttack::tick()
{
}

bool CTurretBasicAttack::Use()
{
	// 서버에게 기본 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// 기본 공격의 레벨은 언제나 1
		SkillType::TURRET_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

	return true;
}

void CTurretBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);
}
