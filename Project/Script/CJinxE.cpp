#include "pch.h"
#include "CJinxE.h"
#include "CUnitScript.h"
#include "CChampionScript.h"
#include "CJinxEScript.h"
#include "CTimedEffect.h"
CJinxE::CJinxE()
{
	m_strSkillName = L"Flame Chompers!";
	m_fCoolDown = 5.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;

	CGameObject* pObj = nullptr;
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\wazak.fbx");
	pObj = pMeshData->Instantiate();
	pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\wazak");
	pObj->AddComponent(new CCollider2D);
	pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	pObj->Collider2D()->SetOffsetScale(Vec2(30.f, 30.f));
	pObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	pObj->Collider2D()->SetDrawCollision(true);
	pObj->SetName(L"JinxE");
	pObj->Transform()->SetRelativeScale(Vec3(0.2f, 0.2f, 0.2f));

	Ptr<CPrefab> NewPrefab = new CPrefab;
	NewPrefab->RegisterProtoObject(pObj);

	m_vecSkillObj.push_back(NewPrefab);
	m_vecSkillObj.push_back(NewPrefab);
	m_vecSkillObj.push_back(NewPrefab);

	// 투사체 스크립트
	m_iProjectileCount = 3;
	m_ProjectileScript = new CJinxEScript;
}

CJinxE::~CJinxE()
{
}

void CJinxE::tick()
{
	CSkill::tick();
}

bool CJinxE::Use()
{
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;
	
	// 서버에게 JinxE 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		UINT64_MAX,		// 논타겟팅일 경우 UINT64_MAX를 써주세요
		m_UserObj->GetScript<CUnitScript>()->GetSkillLevel(3),
		SkillType::JINX_E,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		GetMousePos(),
		true,
		GetMouseDir());


	// 쿨타임 초기화
	m_fCurCoolDown = m_fCoolDown;


    return true;
}

void CJinxE::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _SkillLevel)
{
	// 징크스 E 투사체가 적과 충돌시 이 함수에 본인 스크립트 넣어서 호출할 것임
	CUnitScript* TargetUnitScript = dynamic_cast<CUnitScript*>(_TargetScript);

	// 2초 동안 속박시킵니다.
	CTimedEffect* JinxERoot = new CTimedEffect(TargetUnitScript, 1.5f, 0, 0, CC::ROOT);
	TargetUnitScript->AddTimedEffect(JinxERoot);

	CSkill::GetHit(_UserScript, _TargetScript, _SkillLevel);
}
