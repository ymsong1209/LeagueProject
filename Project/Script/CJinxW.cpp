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
	JinxWAttackObj->AddComponent(new CCollider2D);
	JinxWAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	JinxWAttackObj->Collider2D()->SetAbsolute(true);
	JinxWAttackObj->Collider2D()->SetOffsetScale(Vec2(5.f, 5.f));
	JinxWAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	JinxWAttackObj->Transform()->SetRelativeScale(Vec3(15.99f, 70.f, 1.f));

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = JinxWAttackObj->Clone();
	NewPrefab->RegisterProtoObject(JinxWAttackObj);

	vector<CGameObject*> Childs = JinxWAttackObj->GetChild();

	Childs[0]->Transform()->SetAbsolute(false);
	Childs[0]->Transform()->SetRelativeScale(Vec3(5.f, 6.f, 1.f));

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
	CUnitScript* UnitScript = dynamic_cast<CUnitScript*>(_UserScript);
	if (UnitScript != nullptr)
	{
		float BaseDamage = 50.f;
		int   level = ChamScript->GetLevel();
		float AttackPow = ChamScript->GetAttackPower();

		// 예시입니다
		Damage = BaseDamage + (level * 2) + (AttackPow * 0.3f);
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

	// 2초 동안 둔화시킵니다.
	CTimedEffect* JinxWSlow = new CTimedEffect(_TargetScript, 2.f, 0, 0, CC::SLOW);
	_TargetScript->AddTimedEffect(JinxWSlow);

	// 테스트용 도트딜
	CTimedEffect* TestDot = new CTimedEffect(_TargetScript, 3.f, 5.f, 6, CC::NO_CC);
	_TargetScript->AddTimedEffect(TestDot);
  
  CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);

  // 피격자 사망시 KDACS 패킷 전송
  if (_TargetScript->GetCurHP() <= 0)
	  CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
		  , _TargetScript->GetServerID()
		  , _TargetScript->GetUnitType());
}
