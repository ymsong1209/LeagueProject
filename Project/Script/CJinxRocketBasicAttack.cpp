#include "pch.h"
#include "CJinxRocketBasicAttack.h"

CJinxRocketBasicAttack::CJinxRocketBasicAttack()
{
    m_strSkillName = L"Jinx_Rocket_Basic_Attack";
    m_fCoolDown = 0.f;
    m_iLevel = 1;

	// ����ü (�����Դϴ�)
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
	//
	//m_vecSkillObj.push_back(NewPrefab);
	//
	//// ����ü ��ũ��Ʈ
	//m_iProjectileCount = 1;
	//
	//// �ǰ� ����Ʈ
	//Ptr<CPrefab> MalphiteBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteBasicAttackHitEffect.prefab");
	//m_SkillHitEffect = MalphiteBasicAttackHitEffect;
}

CJinxRocketBasicAttack::~CJinxRocketBasicAttack()
{
}

void CJinxRocketBasicAttack::tick()
{
}

bool CJinxRocketBasicAttack::Use()
{
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
		SkillType::JINX_ROCKET_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

    return false;
}

void CJinxRocketBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);
}
