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
	//m_Skill[0]->SetProjectileObj(); // 투사체 프리팹 설정
	//m_Skill[0]->
	m_SkillLevel[0] = 1;

	// 오브젝트가 현재 챔피언의 사거리 내에 있는지 확인
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

	//포탑 공격 쿨타임
	m_fAttackCoolTime += DT;
	if (m_fAttackCoolTime >= m_fAttackSpeed)
		Attack();

	//타겟 확정나면 공격
	//f (m_pTarget) 
	//
	//	//나중에는 prefab로 소환해야함
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
	//	//포탑 공격의 target설정
	//	script->SetTarget(m_pTarget);
	//	SpawnGameObject(TurretAttack, Vec3(TowerPos.x, TowerPos.y + 50.f, TowerPos.z), L"Projectile");
	//	//공격 구체 소환하고 나서 포탑 쿨타임 초기화
	//	m_fAttackCoolTime = 3.f;
	//
}




void CTurretScript::ChangeAnim()
{
	// 남은 체력에 따라 애니메이션 바꿔줌

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
		// 포탑 Dead 상태
		m_bUnitDead = true;
		// 시야 제공 기능 삭제
		GetOwner()->Transform()->SetIsShootingRay(false);
		
		// 자식 지우기
		CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
		if (TurretBase)
			DestroyObject(TurretBase);
		
		// 자식 애니메이션 재생 후 삭제
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
	// 공격 쿨타임 초기화
	m_fAttackCoolTime = 0;

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
		BasicAttack->SetOwnerScript(this);

		BasicAttack->Use();
	}
}

void CTurretScript::CheckStatus()
{
	// 공격 가능한지 여부 체크

	// 포탑이 파괴되었다면 무조건 공격 불가
	if (m_bUnitDead)
	{
		m_bAttackable = false;
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
	if (!(_obj->GetScript<CUnitScript>()->IsUnitDead()))
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