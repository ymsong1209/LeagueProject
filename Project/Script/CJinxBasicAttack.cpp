#include "pch.h"
#include "CJinxBasicAttack.h"

#include "CDefaultGetHitEffectScript.h"

CJinxBasicAttack::CJinxBasicAttack()
{
	m_strSkillName = L"Jinx_Basic_Attack";
	m_fCoolDown = 0.f;
	m_iLevel = 1;

	m_vecSkillObj.clear();

	CGameObject* JinxBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxBasicAttackEffect.prefab")->Instantiate();
	JinxBasicAttackObj->AddComponent(new CCollider2D);
	JinxBasicAttackObj->Transform()->SetRelativeScale(Vec3(2.4f, 40.f, 1.f));
	JinxBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	JinxBasicAttackObj->Collider2D()->SetAbsolute(true);
	JinxBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
	JinxBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	JinxBasicAttackObj->Collider2D()->SetDrawCollision(true);
	JinxBasicAttackObj->Transform()->SetBillBoard(false);
	JinxBasicAttackObj->MeshRender()->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DEffectShaderAlpha"));
	JinxBasicAttackObj->SetName(L"JinxBasicAttackMinigun");

	vector<CGameObject*> Childs = JinxBasicAttackObj->GetChild();

	Childs[0]->Transform()->SetRelativeScale(Vec3(2.4f, 20.f, 1.f));
	Childs[0]->Transform()->SetRelativePos(Vec3(-3.48f, -25.24f, 8.f));

	Childs[1]->Transform()->SetRelativeScale(Vec3(2.4f, 30.f, 1.f));
	Childs[1]->Transform()->SetRelativePos(Vec3(1.6f, -40.64f, 8.f));

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = JinxBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// �ǰ� ����Ʈ
	Ptr<CPrefab> JinxBasicAttackGetHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxBasicAttackGetHitEffect.prefab");
	JinxBasicAttackGetHitEffect.Get()->GetProtoObject()->AddComponent(new CDefaultGetHitEffectScript);
	JinxBasicAttackGetHitEffect.Get()->GetProtoObject()->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 1.f));
	JinxBasicAttackGetHitEffect.Get()->GetProtoObject()->GetScript<CDefaultGetHitEffectScript>()->SetExpandSpeed(20.f);
	JinxBasicAttackGetHitEffect.Get()->GetProtoObject()->GetScript<CDefaultGetHitEffectScript>()->SetDeleteScale(1.f);




	m_SkillHitEffect = JinxBasicAttackGetHitEffect;





	// ����ü ��ũ��Ʈ
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
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
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
