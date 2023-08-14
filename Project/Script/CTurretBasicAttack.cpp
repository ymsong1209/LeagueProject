#include "pch.h"
#include "CTurretBasicAttack.h"

CTurretBasicAttack::CTurretBasicAttack()
{
	m_strSkillName = L"TurretBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	//// 투사체
	//CGameObject* Projectile = new CGameObject;
	//Projectile->AddComponent(new CTransform);
	//Projectile->AddComponent(new CCollider2D);
	//Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	//Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 5.f));
	//Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	//Projectile->SetName(L"TurretProjectile");
	
	// 이팩트를 그냥 투사체로 만듦(땜빵용)
	Ptr<CPrefab> TurretBlaze = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\TurretBlaze.prefab");
	CGameObject* TurretBlazeObject = TurretBlaze->Instantiate();
	TurretBlazeObject->Transform()->SetRelativeScale(50, 50, 1); // 비율 작게 스케일 조정 //  blaze가 나오기 위한 크기
	TurretBlazeObject->AddComponent(new CCollider2D);
	TurretBlazeObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	TurretBlazeObject->Collider2D()->SetOffsetScale(Vec2(1.f/10, 1.f/10)); // 비율 작게 조정)	 // 충돌 크기는 trasnform의 scale과 기본이 똑같아서 offset으로 원하는 만큼 줄임
	TurretBlazeObject->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	TurretBlazeObject->SetName(L"TurretProjectile");

	// 최종 투사체를 프리팹으로 저장
	Ptr<CPrefab> NewPrefab = new CPrefab;
	//CGameObject* PrefabObject = Projectile->Clone();
	NewPrefab->RegisterProtoObject(TurretBlazeObject);
	
	m_vecSkillObj.push_back(NewPrefab);

	// 투사체 스크립트
	m_iProjectileCount = 1;

	// 이 스킬의 피격 이펙트 등록 (임시로 말파 피격)
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

// 맞은 플레이어가 이 함수를 통해 계산을 함.
void CTurretBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	// 평타 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임

	float Damage = 0;

	// 시전자의 레벨, 기본 공격력 등에 따라 데미지 계산
	CUnitScript* UnitScript = dynamic_cast<CUnitScript*>(_UserScript);
	if (UnitScript != nullptr)
	{
		float BaseDamage = 0.f;
		float AttackPow = UnitScript->GetAttackPower();

		// 예시입니다
		Damage = AttackPow;
	}

	// 데미지에서 타겟의 방어력만큼을 제한 뒤 실제 반영할 데미지 계산
	float DefencePow = _TargetScript->GetDefencePower();

	Damage -= DefencePow;

	float minDam = 10.f;
	if (Damage < minDam)
	{
		// 데미지 최소값
		Damage = minDam;
	}

	_TargetScript->SetCurHPVar(-Damage);


	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);

	// 맞을 시 피격 이펙트 패킷 전송
	//CSendServerEventMgr::GetInst()->어쩌구 (유저, 타겟, 스킬타입 등)

	// 피격자 사망시 KDACS 패킷 전송
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());

	
}
