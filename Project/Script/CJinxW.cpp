#include "pch.h"
#include "CJinxW.h"
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CJinxWScript.h"
#include "CTimedEffect.h"

CJinxW::CJinxW()
{
	m_strSkillName = L"Zap!";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;

	CGameObject* JinxWAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxWEffect.prefab")->Instantiate();
	JinxWAttackObj->SetName(L"JinxWAttack");

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = JinxWAttackObj->Clone();
	NewPrefab->RegisterProtoObject(JinxWAttackObj);

	m_vecSkillObj.push_back(NewPrefab);

	// 투사체 스크립트
	m_iProjectileCount = 1;
	//m_ProjectileScript = new CJinxWScript;
}

CJinxW::~CJinxW()
{
}

void CJinxW::tick()
{
	CSkill::tick();
}

bool CJinxW::Use()
{
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// 서버에게 기본 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// 논타겟팅일 경우 UINT64_MAX를 써주세요
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(2),
		SkillType::JINX_W,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		true,
		GetMouseDir());

	// 쿨타임 초기화
	m_fCurCoolDown = m_fCoolDown;

	return true;
}

void CJinxW::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// 징크스 w 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임

	float Damage = 0;

	// 시전자의 레벨, 기본 공격력 등에 따라 데미지 계산
	CChampionScript* ChamScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChamScript != nullptr)
	{
		float BaseDamage = 50.f;
		int   level		 = ChamScript->GetLevel();
		float AttackPow  = ChamScript->GetAttackPower();

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
	CTimedEffect* TestDot = new CTimedEffect(TargetUnitScript, 3.f, 5.f, 6, CC::NO_CC);
	TargetUnitScript->AddTimedEffect(TestDot);
  
  CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);

  // 피격자 사망시 KDACS 패킷 전송
  if (_TargetScript->GetCurHP() <= 0)
	  CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
		  , _TargetScript->GetServerID()
		  , _TargetScript->GetUnitType());
}
