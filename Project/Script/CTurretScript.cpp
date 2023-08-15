#include "pch.h"
#include "CTurretScript.h"
#include <Engine\CAnim3D.h>
#include <Engine\CAnimator3D.h>
#include "CChampionScript.h"
#include "CBasicAttack.h"
#include "CGameEventMgr.h"
#include "CTurretIdleState.h"
#include "CTurretBrokenState.h"
#include "CTurretBasicAttack.h"


CTurretScript::CTurretScript()
	:CStructureScript((UINT)SCRIPT_TYPE::TURRETSCRIPT)
{
	m_fMaxHP = 10.f;
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
	m_Skill[0] = new CTurretBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	//Ptr<CPrefab> TurretProjectile = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\TurretBlaze.prefab");
	//m_Skill[0]->SetProjectileObj(TurretProjectile); // ����ü ������ ����
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

	if (m_bUnitDead)
		return;
	
	ChangeAnim();

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
		if (CSendServerEventMgr::GetInst()->GetMyPlayer() == m_pAttackTarget)
		{
			CSound* newSound = new CSound;
			wstring filepath = CPathMgr::GetInst()->GetContentPath();
			filepath += L"sound2d\\sfx_TurretTargeting.mp3";
			newSound->Load(filepath);
			CSoundMgr::GetInst()->AddSound(newSound);
			int soundId = newSound->GetSoundIndex();
			CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, GetOwner()->Transform()->GetRelativePos());
			CSoundMgr::GetInst()->Stop(soundId);
			CSoundMgr::GetInst()->Play(soundId, 1, 0.5f, true, 0.f, GetOwner()->Transform()->GetRelativePos());
		}

		CSkill* BasicAttack = GetSkill(0);
		BasicAttack->SetUserObj(this->GetOwner());
		BasicAttack->SetTargetObj(m_pAttackTarget);
		
		BasicAttack->Use();

		// Ÿ�� ���� ����
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\sfx_TurretBasicAttack.mp3", 1, 0.6f, true, 200.f, GetOwner()->Transform()->GetRelativePos(), Faction::NONE);

		// ���� ��Ÿ�� �ʱ�ȭ
		m_fAttackCoolTime = 0;
	}
}

void CTurretScript::CheckStatus()
{
	// ���� �������� ���� üũ

	// ��ž�� �ı��Ǿ��ٸ� ������ ���� �Ұ�
	if (m_fHP <=  0)
	{
		m_bUnitDead = true;
		
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