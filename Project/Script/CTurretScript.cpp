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
	//m_Skill[0]->SetProjectileObj(); // 투사체 프리팹 설정
	m_SkillLevel[0] = 1;

	// 오브젝트가 현재 챔피언의 사거리 내에 있는지 확인
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
	// 사거리 적용
	AttackRange->Collider2D()->SetOffsetScale(Vec2(m_fAttackRange, m_fAttackRange));
	CAttackRangeScript* AttackRangeScript = AttackRange->GetScript<CAttackRangeScript>();
	m_AttackRangeScript = AttackRangeScript;

	// FSM에 State 추가
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

	//포탑 공격 쿨타임
	m_fAttackCoolTime += DT;
	if (m_fAttackCoolTime >= m_fAttackSpeed)
		Attack();
}



void CTurretScript::ChangeAnim()
{
	// 남은 체력에 따라 애니메이션 바꿔줌
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
		// 포탑 Dead 상태
		m_bUnitDead = true;
		// 시야 제공 기능 삭제
		GetOwner()->Transform()->SetIsShootingRay(false);

		// 잔해 재질 켜기
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
		// 자식 지우기
		if (TurretBase != nullptr && !TurretBaseScript->IsUnitDead())
		{
			TurretBaseScript->SetUnitDead(true);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBaseScript->GetServerID(), 0.f);
		}
		
		// 자식 애니메이션 재생 후 삭제
		CGameObject* TurretBreak1 = GetOwner()->FindChildObjByName(L"TurretBreak_1");
		CUnitScript* TurretBreak1Script = TurretBreak1->GetScript<CUnitScript>();
		if (TurretBreak1)
		{
			// TurretBreak1 재질 켜기
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
			// TurretBreak2 재질 켜기
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
	// 공격 타겟이 없거나, 유효하지 않거나(죽었거나, 사거리에 더이상 존재하지 않는다면), 아군 챔피언을 공격한 적군이 있다면
	if (!m_pAttackTarget || !IsValidTarget(m_pAttackTarget) || IsChampionAttackedAllyInTurretRange())
	{
		// 새로운 타겟 탐색
		SelectTarget();
	}

	if (m_pAttackTarget)
	{
		CSkill* BasicAttack = GetSkill(0);
		BasicAttack->SetUserObj(this->GetOwner());
		BasicAttack->SetTargetObj(m_pAttackTarget);
		
		BasicAttack->Use();

		// 공격 쿨타임 초기화
		m_fAttackCoolTime = 0;
	}
}

void CTurretScript::CheckStatus()
{
	// 공격 가능한지 여부 체크

	// 포탑이 파괴되었다면 무조건 공격 불가
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
	// 사거리 내의 유닛들을 가져옴
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
			// 아군을 공격한 챔피언이 있을 경우, 1순위로 타겟
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

		// 아군 공격 챔피언이 없는 경우, 몹이 2순위
		if (MobTarget)
		{
			m_pAttackTarget = MobTarget;
		}
		// 몹도 없는 경우, 아군 공격하지 않은 챔피언이 3순위
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

	// 포탑 사거리 내의 오브젝트
	vector<CGameObject*> UnitinRange = m_AttackRangeScript->GetUnitsInRange();
	auto it = find(UnitinRange.begin(), UnitinRange.end(), _obj);

	// 포탑 사거리 내부에 더이상 그 오브젝트가 없다면
	if (it == UnitinRange.end())
		return false;

	return true;
}

bool CTurretScript::IsChampionAttackedAllyInTurretRange()
{
	// 사거리 내의 유닛들을 가져옴
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


/* 어떤 것을 본체(부모, 이 스크립트가 붙을 객체)로 할 것인가?

1. IdleBreak

장점1: 마우스 오버레이시 포탑 전체에 테두리쳐짐
장점2: idle, break1, break2 자체를 가지고 있어서 바꿔주기 쉬움

단점1: 포탑이 죽고 난 뒤에 처리?
	   스스로를(이 스크립트 포함) 그냥 삭제해버리기? (마치 맵의 일부처럼 그냥 모델만 남음)

	   아니면 안보이기?
	   이 경우 클릭 불가능하게 처리가 가능할까?


2. Rubble

장점1: 모든 상태에 Rubble이 있어도 됨으로 끝까지 포탑 로직을 책임질수 있음
	   포탑 죽으면 자식들의 애님이 끝나고  전부 다 밀어버려도 됨

단점1: 본인이 Anim을 가진게 아니기 때문에, 자식의 Anim을 돌려줘야함. 약간 불편
단점2: 마우스 오버레이시 포탑 전체가 아니라 잔해에만 외곽선 나타날수도 있음


*/