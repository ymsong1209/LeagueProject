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

	CGameObject* Projectile = new CGameObject;
	Projectile->AddComponent(new CTransform);
	Projectile->AddComponent(new CCollider2D);
	Projectile->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
	Projectile->Collider2D()->SetOffsetScale(Vec2(5.f, 20.f));
	Projectile->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	Projectile->Collider2D()->SetDrawCollision(true);
	Projectile->SetName(L"UnstoppableForce");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = Projectile->Clone();
	NewPrefab->RegisterProtoObject(Projectile);

	m_vecSkillObj.push_back(NewPrefab);

	// 투사체 스크립트
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
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// 서버에게 기본 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// 논타겟팅일 경우 UINT64_MAX를 써주세요
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(2),
		SkillType::MALPHITE_E,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0.f,0.f,0.f));

	// 쿨타임 초기화
	m_fCurCoolDown = m_fCoolDown;

	return true;
}

void CMalphiteE::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// 징크스 w 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임

	float Damage = 0;

	// 시전자의 레벨, 기본 공격력 등에 따라 데미지 계산
	CChampionScript* ChamScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChamScript != nullptr)
	{
		float BaseDamage = 50.f;
		int   level = ChamScript->GetLevel();
		float AttackPow = ChamScript->GetAttackPower();

		// 예시입니다
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
			// 데미지 최소값
			Damage = minDam;
		}
	}

	TargetUnitScript->SetCurHPVar(-Damage);

	// 2초 동안 둔화시킵니다.
	CTimedEffect* JinxWSlow = new CTimedEffect(TargetUnitScript, 2.f, 0, 0, CC::SLOW);
	TargetUnitScript->AddTimedEffect(JinxWSlow);

	// 테스트용 도트딜
	CTimedEffect* TestDot = new CTimedEffect(TargetUnitScript, 3.f, 5.f, 6, CC::CLEAR);
	TargetUnitScript->AddTimedEffect(TestDot);

	CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);

	// 피격자 사망시 KDACS 패킷 전송
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
