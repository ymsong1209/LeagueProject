#include "pch.h"
#include "CRangedMinionBasicAttack.h"
#include "CDefaultGetHitEffectScript.h"

CRangedMinionBasicAttack::CRangedMinionBasicAttack()
{
	m_strSkillName = L"RangedMinionBasicAttack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	CGameObject* RangedMinionBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\RedMinionBasicAttack.prefab")->Instantiate();
	RangedMinionBasicAttackObj->SetName(L"RedRangedMinionBasicAttackProjectile");
	//RangedMinionBasicAttackObj->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Minion\\MinionRedAttack.dds"));



	// 피격 이펙트
	Ptr<CPrefab> MinionBasicAttackHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MinionRedGetHitByBasicAttack.prefab");
	MinionBasicAttackHitEffect.Get()->GetProtoObject()->AddComponent(new CDefaultGetHitEffectScript);
	MinionBasicAttackHitEffect.Get()->GetProtoObject()->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));
	MinionBasicAttackHitEffect.Get()->GetProtoObject()->GetScript<CDefaultGetHitEffectScript>()->SetExpandSpeed(20.f);
	MinionBasicAttackHitEffect.Get()->GetProtoObject()->GetScript<CDefaultGetHitEffectScript>()->SetDeleteScale(1.f);

	m_SkillHitEffect = MinionBasicAttackHitEffect;

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = RangedMinionBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// 투사체 스크립트
	m_iProjectileCount = 1;
}

CRangedMinionBasicAttack::~CRangedMinionBasicAttack()
{

}

void CRangedMinionBasicAttack::tick()
{
	
}

bool CRangedMinionBasicAttack::Use()
{
	// 서버에게 기본 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// 기본 공격의 레벨은 언제나 1
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
