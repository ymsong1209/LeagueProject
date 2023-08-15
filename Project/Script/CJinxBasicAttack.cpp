#include "pch.h"
#include "CJinxBasicAttack.h"

CJinxBasicAttack::CJinxBasicAttack()
{
	m_strSkillName = L"Jinx_Basic_Attack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	CGameObject* JinxBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxBasicAttackEffect.prefab")->Instantiate();
	JinxBasicAttackObj->AddComponent(new CCollider2D);
	JinxBasicAttackObj->Transform()->SetRelativeScale(Vec3(20.f, 100.f, 1.f));
	JinxBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	JinxBasicAttackObj->Collider2D()->SetAbsolute(true);
	JinxBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
	JinxBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	JinxBasicAttackObj->Collider2D()->SetDrawCollision(true);
	JinxBasicAttackObj->Transform()->SetBillBoard(false);
	JinxBasicAttackObj->MeshRender()->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DEffectShaderAlpha"));
	JinxBasicAttackObj->SetName(L"JinxBasicAttackMinigun");

	vector<CGameObject*> Childs = JinxBasicAttackObj->GetChild();
	for (int i = 0; i < Childs.size(); ++i)
	{
		Childs[i]->Transform()->SetRelativeScale(Vec3(20.f, 100.f, 1.f));
	}

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = JinxBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	//Ptr<CPrefab> MalphiteEEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\MalphiteEDecal.prefab");
	//m_SkillHitEffect = MalphiteEEffect;

	// 투사체 스크립트
	m_iProjectileCount = 1;

	m_eSkillType = SkillType::JINX_BASIC_ATTACK;
}

CJinxBasicAttack::~CJinxBasicAttack()
{
}

void CJinxBasicAttack::tick()
{
}

bool CJinxBasicAttack::Use()
{
	// 서버에게 기본 공격 사용 신호를 전달
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// 기본 공격의 레벨은 언제나 1
		SkillType::JINX_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

	return true;
}

void CJinxBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);
}
