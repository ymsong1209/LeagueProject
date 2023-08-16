#include "pch.h"
#include "CDragonBasicAttack.h"
#include "CImmediateGetHitScript.h"

CDragonBasicAttack::CDragonBasicAttack()
{
	m_strSkillName = L"DragonBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	// 투사체 생성
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

	// 피격 이펙트
	Ptr<CPrefab> DragonBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteBasicAttackHitEffect.prefab");
	DragonBasicAttackHitEffect.Get()->GetProtoObject()->AddComponent(new CImmediateGetHitScript);
 

	m_SkillHitEffect = DragonBasicAttackHitEffect;

	// 투사체 스크립트
	m_iProjectileCount = 1;
}

CDragonBasicAttack::~CDragonBasicAttack()
{

}

void CDragonBasicAttack::tick()
{
	// 쿨타임 계산 필요 없음
}

bool CDragonBasicAttack::Use()
{
	// 서버에게 기본 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// 기본 공격의 레벨은 언제나 1
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


	CSkill::GetHit(_UserScript, _TargetScript, _Skilllevel);

	// 맞을 시 피격 이펙트 패킷 전송
	//CSendServerEventMgr::GetInst()->어쩌구 (유저, 타겟, 스킬타입 등)

	// 피격자 사망시 KDACS 패킷 전송
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
