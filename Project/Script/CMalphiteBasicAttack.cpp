#include "pch.h"
#include "CMalphiteBasicAttack.h"

CMalphiteBasicAttack::CMalphiteBasicAttack()
{
	m_strSkillName = L"BaseAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	// 투사체
	CGameObject* Projectile = new CGameObject;
	Projectile->AddComponent(new CTransform);
	Projectile->AddComponent(new CCollider2D);
	Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 5.f));
	Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	Projectile->SetName(L"Projectile");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = Projectile->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// 투사체 스크립트
	m_iProjectileCount = 1;

	// 피격 이펙트
	Ptr<CPrefab> MalphiteBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\BasicAttack.prefab");
	m_SkillHitEffect = MalphiteBasicAttackHitEffect;
}

CMalphiteBasicAttack::~CMalphiteBasicAttack()
{
}

void CMalphiteBasicAttack::tick()
{
}

bool CMalphiteBasicAttack::Use()
{
	CBasicAttack::Use();
}

void CMalphiteBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);
}
