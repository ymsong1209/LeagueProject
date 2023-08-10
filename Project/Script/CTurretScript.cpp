#include "pch.h"
#include "CTurretScript.h"
#include <Engine\CAnim3D.h>
#include <Engine\CAnimator3D.h>
#include "CChampionScript.h"
#include "CTurretAttackScript.h"
#include "CBasicAttack.h"

CTurretScript::CTurretScript()
	:CStructureScript((UINT)SCRIPT_TYPE::TURRETSCRIPT)
{
	m_eFaction = Faction::RED;

	m_fAttackPower = 1;
	m_fDefencePower = 0;
	m_fAttackSpeed = 2.f;
	m_fAttackRange = 300;
	m_fMoveSpeed = 0;
}

CTurretScript::~CTurretScript()
{

}

void CTurretScript::begin()
{
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	//m_Skill[0]->SetProjectileObj(); // ����ü ������ ����
	//m_Skill[0]->
	m_SkillLevel[0] = 1;

	// ������Ʈ�� ���� è�Ǿ��� ��Ÿ� ���� �ִ��� Ȯ��
	CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
	CAttackRangeScript* AttackRangeScript = AttackRange->GetScript<CAttackRangeScript>();
	m_AttackRangeScript = AttackRangeScript;

	CStructureScript::begin();
}

void CTurretScript::tick()
{
	CStructureScript::tick();

	CheckStatus();
	ChangeAnim();

	if (m_bUnitDead)
		return;

	//��ž ���� ��Ÿ��
	m_fAttackCoolTime += DT;
	if (m_fAttackCoolTime >= m_fAttackSpeed)
		Attack();

	//Ÿ�� Ȯ������ ����
	//f (m_pTarget) 
	//
	//	//���߿��� prefab�� ��ȯ�ؾ���
	//	//CGameObject* TurretAttack = CResMgr::GetInst()->FindRes<CPrefab>(L"TurretAttack")->Instantiate();
	//	CGameObject* TurretAttack = new CGameObject;
	//	TurretAttack->SetName(L"TurretAttack");
	//	TurretAttack->AddComponent(new CTransform);
	//	TurretAttack->AddComponent(new CTurretAttackScript);
	//	TurretAttack->AddComponent(new CMeshRender);
	//	TurretAttack->Transform()->SetRelativeScale(Vec3(20.f, 20.f, 20.f));
	//	TurretAttack->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	//	TurretAttack->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	//	Vec3 TowerPos = GetOwner()->Transform()->GetRelativePos();
	//	CTurretAttackScript* script = TurretAttack->GetScript<CTurretAttackScript>();
	//	//��ž ������ target����
	//	script->SetTarget(m_pTarget);
	//	SpawnGameObject(TurretAttack, Vec3(TowerPos.x, TowerPos.y + 50.f, TowerPos.z), L"Projectile");
	//	//���� ��ü ��ȯ�ϰ� ���� ��ž ��Ÿ�� �ʱ�ȭ
	//	m_fAttackCoolTime = 3.f;
	//
}




void CTurretScript::ChangeAnim()
{
	// ���� ü�¿� ���� �ִϸ��̼� �ٲ���

	float HealthRatio = m_fHP / m_fMaxHP;
	if (HealthRatio >= 0.66f)
	{
		CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
		if (TurretBase->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Idle")
			TurretBase->Animator3D()->PlayRepeat(L"turret_idlebreak\\Turret_Idle", false);
	}
	else if (33.f < HealthRatio && HealthRatio <= 0.66f)
	{
		CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
		if (TurretBase->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Cloth_Break1")
			TurretBase->Animator3D()->PlayOnce(L"turret_idlebreak\\Turret_Cloth_Break1", false);
	}
	else if (0 < HealthRatio && HealthRatio <= 33.f)
	{
		CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
		if (TurretBase->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Cloth_Break2")
			TurretBase->Animator3D()->PlayOnce(L"turret_idlebreak\\Turret_Cloth_Break2", false);
	}
	else
	{
		// ��ž Dead ����
		m_bUnitDead = true;
		// �þ� ���� ��� ����
		GetOwner()->Transform()->SetIsShootingRay(false);
		
		// �ڽ� �����
		CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
		if (TurretBase)
			DestroyObject(TurretBase);
		
		// �ڽ� �ִϸ��̼� ��� �� ����
		CGameObject* TurretBreak1 = GetOwner()->FindChildObjByName(L"TurretBreak_1");
		if (TurretBreak1)
		{
			if(TurretBreak1->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Cloth_Break1")
				TurretBreak1->Animator3D()->PlayOnce(L"turret_idlebreak\\Turret_Cloth_Break1", false);
			else
			{
				if (TurretBreak1->Animator3D()->GetCurAnim()->IsFinish())
					DestroyObject(TurretBreak1);
			}
		}

		CGameObject* TurretBreak2 = GetOwner()->FindChildObjByName(L"TurretBreak_2");
		if (TurretBreak1)
		{
			if (TurretBreak1->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Cloth_Break2")
				TurretBreak1->Animator3D()->PlayOnce(L"turret_idlebreak\\Turret_Cloth_Break2", false);
			else
			{
				if (TurretBreak2->Animator3D()->GetCurAnim()->IsFinish())
					DestroyObject(TurretBreak2);
			}
		}
	}
}

void CTurretScript::Attack()
{
	// ���� ��Ÿ�� �ʱ�ȭ
	m_fAttackCoolTime = 0;

	// ���� Ÿ���� ���ų�, ��ȿ���� �ʰų�(�׾��ų�, ��Ÿ��� ���̻� �������� �ʴ´ٸ�), �Ʊ� è�Ǿ��� ������ ������ �ִٸ�
	if (!m_pAttackTarget || !IsValidTarget(m_pAttackTarget) || IsChampionAttackedAllyInTurretRange())
	{
		// ���ο� Ÿ�� Ž��
		SelectTarget();
	}

	if (m_pAttackTarget)
	{
		CSkill* BasicAttack = GetSkill(0);
		BasicAttack->SetUserObj(this->GetOwner());
		BasicAttack->SetTargetObj(m_pAttackTarget);
		BasicAttack->SetOwnerScript(this);

		BasicAttack->Use();
	}
}

void CTurretScript::CheckStatus()
{
	// ���� �������� ���� üũ

	// ��ž�� �ı��Ǿ��ٸ� ������ ���� �Ұ�
	if (m_bUnitDead)
	{
		m_bAttackable = false;
		return;
	}
}

void CTurretScript::SelectTarget()
{
	// ��Ÿ� ���� ���ֵ��� ������
	vector<CGameObject*> UnitinRange = m_AttackRangeScript->GetUnitsInRange();

	CGameObject* MobTarget = nullptr;
	CGameObject* ChampionTarget = nullptr;

	if (UnitinRange.size() == 0)
	{
		m_pAttackTarget = nullptr;
		return;
	}
	else
	{
		for (auto& target : UnitinRange)
		{
			// �Ʊ��� ������ è�Ǿ��� ���� ���, 1������ Ÿ��
			CChampionScript* Champion = dynamic_cast<CChampionScript*>(target);
			if (Champion != nullptr && Champion->IsAttackingChampion())
			{
				m_pAttackTarget = target;
				return;
			}

			else if (Champion && !ChampionTarget)
			{
				ChampionTarget = target;
			}
			else if (!MobTarget)
			{
				MobTarget = target;
			}
		}

		// �Ʊ� ���� è�Ǿ��� ���� ���, ���� 2����
		if (MobTarget)
		{
			m_pAttackTarget = MobTarget;
		}
		// ���� ���� ���, �Ʊ� �������� ���� è�Ǿ��� 3����
		else if (ChampionTarget)
		{
			m_pAttackTarget = ChampionTarget;
		}

	}
}

bool CTurretScript::IsValidTarget(CGameObject* _obj)
{
	if (!(_obj->GetScript<CUnitScript>()->IsUnitDead()))
		return false;

	// ��ž ��Ÿ� ���� ������Ʈ
	vector<CGameObject*> UnitinRange = m_AttackRangeScript->GetUnitsInRange();
	auto it = find(UnitinRange.begin(), UnitinRange.end(), _obj);

	// ��ž ��Ÿ� ���ο� ���̻� �� ������Ʈ�� ���ٸ�
	if (it == UnitinRange.end())
		return false;

	return true;
}

bool CTurretScript::IsChampionAttackedAllyInTurretRange()
{
	// ��Ÿ� ���� ���ֵ��� ������
	vector<CGameObject*> UnitinRange = m_AttackRangeScript->GetUnitsInRange();

	for (auto& target : UnitinRange)
	{
		CChampionScript* Champion = dynamic_cast<CChampionScript*>(target);
		if (Champion != nullptr && Champion->IsAttackingChampion())
		{
			return true;
		}
	}
	return false;
}


/* � ���� ��ü(�θ�, �� ��ũ��Ʈ�� ���� ��ü)�� �� ���ΰ�?

1. IdleBreak

����1: ���콺 �������̽� ��ž ��ü�� �׵θ�����
����2: idle, break1, break2 ��ü�� ������ �־ �ٲ��ֱ� ����

����1: ��ž�� �װ� �� �ڿ� ó��?
	   �����θ�(�� ��ũ��Ʈ ����) �׳� �����ع�����? (��ġ ���� �Ϻ�ó�� �׳� �𵨸� ����)

	   �ƴϸ� �Ⱥ��̱�?
	   �� ��� Ŭ�� �Ұ����ϰ� ó���� �����ұ�?


2. Rubble

����1: ��� ���¿� Rubble�� �־ ������ ������ ��ž ������ å������ ����
	   ��ž ������ �ڽĵ��� �ִ��� ������  ���� �� �о������ ��

����1: ������ Anim�� ������ �ƴϱ� ������, �ڽ��� Anim�� ���������. �ణ ����
����2: ���콺 �������̽� ��ž ��ü�� �ƴ϶� ���ؿ��� �ܰ��� ��Ÿ������ ����


*/