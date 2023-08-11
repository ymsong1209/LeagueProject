#include "pch.h"
#include "CTurretScript.h"
#include <Engine\CAnim3D.h>
#include <Engine\CAnimator3D.h>
#include "CChampionScript.h"
#include "CTurretAttackScript.h"
#include "CBasicAttack.h"
#include "CGameEventMgr.h"
#include "CTurretIdleState.h"
#include "CTurretBrokenState.h"

CTurretScript::CTurretScript()
	:CStructureScript((UINT)SCRIPT_TYPE::TURRETSCRIPT)
{
	m_fMaxHP = 50.f;
	m_fAttackPower = 1;
	m_fDefencePower = 0;
	m_fAttackSpeed = 2.f;
	m_fAttackRange = 200;
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
	m_SkillLevel[0] = 1;

	// ������Ʈ�� ���� è�Ǿ��� ��Ÿ� ���� �ִ��� Ȯ��
	CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"TurretAttackRange");
	if (AttackRange == nullptr) 
	{
		AttackRange = new CGameObject;
		AttackRange->AddComponent(new CTransform);
		AttackRange->AddComponent(new CCollider2D);
		AttackRange->AddComponent(new CAttackRangeScript);

		AttackRange->SetName(L"TurretAttackRange");
		AttackRange->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		AttackRange->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		AttackRange->Collider2D()->SetAbsolute(true);
		AttackRange->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
		GetOwner()->AddChild(AttackRange);
		AttackRange->ChangeLayer(L"AttackRange");
	}
	// ��Ÿ� ����
	AttackRange->Collider2D()->SetOffsetScale(Vec2(m_fAttackRange, m_fAttackRange));
	CAttackRangeScript* AttackRangeScript = AttackRange->GetScript<CAttackRangeScript>();
	m_AttackRangeScript = AttackRangeScript;

	// FSM�� State �߰�
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	GetOwner()->Fsm()->AddState(L"Idle", new CTurretIdleState);
	GetOwner()->Fsm()->AddState(L"Broken", new CTurretBrokenState);
	GetOwner()->Fsm()->ChangeState(L"Idle");

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
}



void CTurretScript::ChangeAnim()
{
	// ���� ü�¿� ���� �ִϸ��̼� �ٲ���
	CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
	CUnitScript* TurretBaseScript = TurretBase->GetScript<CUnitScript>();

	float HealthRatio = m_fHP / m_fMaxHP;
	if (HealthRatio >= 0.66f)
	{
		if (TurretBase->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Idle")
		{
			TurretBase->Animator3D()->PlayRepeat(L"turret_idlebreak\\Turret_Idle", false);

			CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBaseScript->GetServerID()
			, L"turret_idlebreak\\Turret_Idle"
				, true
				, true
				, false
				, 0.f);
		}
	}
	else if (0.33f < HealthRatio && HealthRatio <= 0.66f)
	{
		if (TurretBase->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Cloth_Break1")
		{
			TurretBase->Animator3D()->PlayOnce(L"turret_idlebreak\\Turret_Cloth_Break1", false);

			CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBaseScript->GetServerID()
				, L"turret_idlebreak\\Turret_Cloth_Break1"
				, false
				, false
				, false
				, 0.f
				, 1.f);
		}
	}
	else if (0 < HealthRatio && HealthRatio <= 0.33f)
	{
		if (TurretBase->Animator3D()->GetCurAnim()->GetName() != L"turret_idlebreak\\Turret_Cloth_Break2")
		{
			TurretBase->Animator3D()->PlayOnce(L"turret_idlebreak\\Turret_Cloth_Break2", false);

			CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBaseScript->GetServerID()
				, L"turret_idlebreak\\Turret_Cloth_Break2"
				, false
				, false
				, false
				, 0.f
			, 1.f);
		}
	}
	else
	{
		// ��ž Dead ����
		m_bUnitDead = true;
		// �þ� ���� ��� ����
		GetOwner()->Transform()->SetIsShootingRay(false);

		// ���� ���� �ѱ�
		if (m_eFaction == Faction::RED)
		{
			Ptr<CTexture> RedTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm.dds");

			GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_red.mtrl"), 0);
			GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_red.mtrl"), 1);
			//GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, RedTex);
			//GetOwner()->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, RedTex);
		
			//CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretBaseScript->GetServerID(), 0, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm.dds");
			//CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretBaseScript->GetServerID(), 1, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm.dds");
		}
		else
		{
			Ptr<CTexture> BlueTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm_blue.dds");

			GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, BlueTex);
			GetOwner()->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, BlueTex);

			CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretBaseScript->GetServerID(), 0, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm_blue.dds");
			CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretBaseScript->GetServerID(), 1, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm_blue.dds");
		}
		// �ڽ� �����
		if (TurretBase != nullptr && !TurretBaseScript->IsUnitDead())
		{
			TurretBaseScript->SetUnitDead(true);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBaseScript->GetServerID(), 0.f);
		}
		
		// �ڽ� �ִϸ��̼� ��� �� ����
		CGameObject* TurretBreak1 = GetOwner()->FindChildObjByName(L"TurretBreak_1");
		CUnitScript* TurretBreak1Script = TurretBreak1->GetScript<CUnitScript>();
		if (TurretBreak1)
		{
			// TurretBreak1 ���� �ѱ�
			TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_red.mtrl"), 0);
			TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_red.mtrl"), 1);

			if (TurretBreak1->Animator3D()->GetCurAnim()->IsPause())
			{
				TurretBreak1->Animator3D()->PlayOnce(L"turret_break1\\turret_break1", false);
				
				CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBreak1Script->GetServerID(),
					L"turret_break1\\turret_break1"
					, false
					, false
					, false
					, 0.f);
			}
			else
			{
				if (TurretBreak1 && !TurretBreak1Script->IsUnitDead() && TurretBreak1->Animator3D()->GetCurAnim()->IsFinish())
				{
					TurretBreak1Script->SetUnitDead(true);
					CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBreak1Script->GetServerID(), 0.f);
				}
			}
		}

		CGameObject* TurretBreak2 = GetOwner()->FindChildObjByName(L"TurretBreak_2");
		CUnitScript* TurretBreak2Script = TurretBreak2->GetScript<CUnitScript>();
		if (TurretBreak2)
		{
			// TurretBreak2 ���� �ѱ�
			TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_red.mtrl"), 0);
			TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_red.mtrl"), 1);

			if (TurretBreak2->Animator3D()->GetCurAnim()->IsPause())
			{
				TurretBreak2->Animator3D()->PlayOnce(L"turret_break2\\turret_break2", false);

				CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBreak2Script->GetServerID(),
					L"turret_break2\\turret_break2"
					, false
					, false
					, false
					, 0.f);
			}
			else
			{
				if (TurretBreak2 && !TurretBreak2Script->IsUnitDead() && TurretBreak2->Animator3D()->GetCurAnim()->IsFinish())
				{
					TurretBreak2Script->SetUnitDead(true);
					CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBreak2Script->GetServerID(), 0.f);
				}
			}
		}
	de
}

void CTurretScript::Attack()
{
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
		
		BasicAttack->Use();

		// ���� ��Ÿ�� �ʱ�ȭ
		m_fAttackCoolTime = 0;
	}
}

void CTurretScript::CheckStatus()
{
	// ���� �������� ���� üũ

	// ��ž�� �ı��Ǿ��ٸ� ������ ���� �Ұ�
	if (m_bUnitDead)
	{
		m_bAttackable = false;
		
		if (GetOwner()->Fsm()->GetCurState()->GetName() != L"Broken")
			GetOwner()->Fsm()->ChangeState(L"Broken");
		
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
	if (_obj->IsDead() || !(_obj->GetScript<CUnitScript>()->IsUnitDead()))
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