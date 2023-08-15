#include "pch.h"
#include "CJinxRocketBasicAttack.h"

CJinxRocketBasicAttack::CJinxRocketBasicAttack()
{
    m_strSkillName = L"Jinx_Rocket_Basic_Attack";
    m_fCoolDown = 0.f;
    m_iLevel = 1;

	m_vecSkillObj.clear();

	CGameObject* JinxBasicAttackObj = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxBasicRocketAttackEffect.prefab")->Instantiate();
	JinxBasicAttackObj->AddComponent(new CCollider2D);
	//JinxBasicAttackObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	JinxBasicAttackObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	JinxBasicAttackObj->Collider2D()->SetAbsolute(true);
	JinxBasicAttackObj->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));
	JinxBasicAttackObj->Collider2D()->SetOffsetRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	JinxBasicAttackObj->Collider2D()->SetDrawCollision(true);
	JinxBasicAttackObj->Transform()->SetBillBoard(false);
	JinxBasicAttackObj->MeshRender()->GetMaterial(0)->SetShader(CResMgr::GetInst()->FindRes<CGraphicsShader>(L"Std2DEffectShaderAlpha"));
	JinxBasicAttackObj->SetName(L"JinxBasicAttackRocket");

	vector<CGameObject*> Childs = JinxBasicAttackObj->GetChild();
	for (int i = 0; i < Childs.size(); ++i)
	{
		Childs[i]->Transform()->SetRelativeScale(Vec3(20.f, 100.f, 1.f));
	}

	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = JinxBasicAttackObj->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);

	m_vecSkillObj.push_back(NewPrefab);

	// �ǰ� ����Ʈ
	CGameObject* JinxBasicAttackGetHitEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\JinxGetHitByRocketAttack.prefab")->Instantiate();
	JinxBasicAttackGetHitEffect->ParticleSystem()->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\jinxtex\\JinxGetHitByRocket.png"));
	Ptr<CPrefab> NewPrefab = new CPrefab;
	CGameObject* PrefabObject = JinxBasicAttackGetHitEffect->Clone();
	NewPrefab->RegisterProtoObject(PrefabObject);
	
	m_SkillHitEffect = NewPrefab;

	// ����ü ��ũ��Ʈ
	m_iProjectileCount = 1;

	m_eSkillType = SkillType::JINX_ROCKET_BASIC_ATTACK;
}

CJinxRocketBasicAttack::~CJinxRocketBasicAttack()
{
}

void CJinxRocketBasicAttack::tick()
{
}

bool CJinxRocketBasicAttack::Use()
{
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
		SkillType::JINX_ROCKET_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

    return false;
}

void CJinxRocketBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);
}
