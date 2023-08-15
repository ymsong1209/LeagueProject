#include "pch.h"
#include "CJinxR.h""
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CJinxRScript.h"
#include "CTimedEffect.h"

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

	// 피격 이펙트
	Ptr<CPrefab> JinxBasicAttackGetHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxGetHitByRocketAttack.prefab");
	m_SkillHitEffect = JinxBasicAttackGetHitEffect;

	// 투사체 스크립트
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
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// 서버에게 궁 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// 논타겟팅일 경우 UINT64_MAX를 써주세요
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(4),
		SkillType::JINX_R,
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

void CJinxR::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// 징크스 R 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임

	float Damage = 0;

	// 시전자의 레벨, 기본 공격력 등에 따라 데미지 계산
	CChampionScript* ChamScript = dynamic_cast<CChampionScript*>(_UserScript);
	if (ChamScript != nullptr)
	{
		float BaseDamage = 70.f;
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

	// 피격자 사망시 KDACS 패킷 전송
	if (_TargetScript->GetCurHP() <= 0)
		CSendServerEventMgr::GetInst()->SendKDACSPacket(_UserScript->GetServerID()
			, _TargetScript->GetServerID()
			, _TargetScript->GetUnitType());
}
