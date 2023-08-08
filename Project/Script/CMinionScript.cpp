#include "pch.h"
#include "CMinionScript.h"
#include "CMinionAttackState.h"
#include "CMinionDeathState.h"
#include "CMinionWalkState.h"
#include "CMinionChaseState.h"
#include "CBasicAttack.h"
#include "CAttackRangeScript.h"

CMinionScript::CMinionScript()
	:CMobScript((UINT)SCRIPT_TYPE::MINIONSCRIPT)
	, m_vecWayPoint{}
	, m_fAggroTime(2.f)
{
}

CMinionScript::~CMinionScript()
{
}

void CMinionScript::begin()
{
	GetOwner()->Transform()->SetUseMouseOutline(true);

	m_eRestraint = RESTRAINT::DEFAULT;

	// test
	//SetFaction(Faction::BLUE);
	//SetLane(Lane::TOP);
	//SetMinionType(MinionType::RANGED);

	// 미니언 타입 별 정보 세팅
	switch (m_eUnitType)
	{
	case UnitType::MELEE_MINION:
	{
		m_fAttackPower = 1.f;
		m_fAttackRange = 30.f;
		m_fAttackSpeed = 1.5f;
		m_fMoveSpeed = 50.f;
		m_fMaxHP = 50.f;
	}
	break;
	case UnitType::RANGED_MINION:
	{
		m_fAttackPower = 2.f;
		m_fAttackRange = 100.f;
		m_fAttackSpeed = 2.5f;
		m_fMoveSpeed = 50.f;
		m_fMaxHP = 50.f;
	}
	break;
	case UnitType::SIEGE_MINION:
	{
		m_fAttackPower = 3.f;
		m_fAttackRange = 80.f;
		m_fAttackSpeed = 2.5f;
		m_fMoveSpeed = 50.f;
		m_fMaxHP = 50.f;
	}
	break;
	case UnitType::SUPER_MINION:
	{
		m_fAttackPower = 5.f;
		m_fAttackRange = 50.f;
		m_fAttackSpeed = 2.f;
		m_fMoveSpeed = 50.f;
		m_fMaxHP = 50.f;
	}
	break;
	}

	//test
	//m_fMoveSpeed = 200.f;
	// 본인의 Lane, MinionType에 따라 정보 변경
	switch (m_eLane)
	{
	case Lane::TOP:
	{
		// Top 동선에서 지나야할 곳 m_vecWayPoint로 pushback
		if (m_eFaction == Faction::BLUE)
		{
			m_vecWayPoint.push_back(Vec3(170.f, 15.f, 754.f));
			m_vecWayPoint.push_back(Vec3(318.f, 15.f, 1862.f));
			m_vecWayPoint.push_back(Vec3(885.f, 15.f, 2013.f));
			m_vecWayPoint.push_back(Vec3(1420.f, 15.f, 2013.f));
			m_vecWayPoint.push_back(Vec3(1880.f, 15.f, 1988.f));
		}
		else if (m_eFaction == Faction::RED)
		{
			m_vecWayPoint.push_back(Vec3(1716.f, 15.f, 2056.f));
			m_vecWayPoint.push_back(Vec3(1422.f, 15.f, 2019.f));
			m_vecWayPoint.push_back(Vec3(311.f, 15.f, 1836.f));
			m_vecWayPoint.push_back(Vec3(170.f, 15.f, 761.f));
			m_vecWayPoint.push_back(Vec3(182.f, 15.f, 297.f));
		}
	}
	break;
	case Lane::MID:
	{
		// Mid 동선에서 지나야할 곳 m_vecWayPoint로 pushback
		if (m_eFaction == Faction::BLUE)
		{
			m_vecWayPoint.push_back(Vec3(395.f, 15.f, 377.f));
			m_vecWayPoint.push_back(Vec3(607.f, 15.f, 618.f));
			m_vecWayPoint.push_back(Vec3(1080.f, 15.f, 1080.f));
			m_vecWayPoint.push_back(Vec3(1566.f, 15.f, 1581.f));
			m_vecWayPoint.push_back(Vec3(1900.f, 15.f, 1900.f));
		}
		else if (m_eFaction == Faction::RED)
		{
			m_vecWayPoint.push_back(Vec3(1561.f, 15.f, 1569.f));
			m_vecWayPoint.push_back(Vec3(1101.f, 15.f, 1087.f));
			m_vecWayPoint.push_back(Vec3(620.f, 15.f, 649.f));
			m_vecWayPoint.push_back(Vec3(421.f, 15.f, 438.f));
			m_vecWayPoint.push_back(Vec3(291.f, 15.f, 299.f));
		}
	}
	break;
	case Lane::BOTTOM:
	{
		// Bot 동선에서 지나야할 곳 m_vecWayPoint로 pushback
		if (m_eFaction == Faction::BLUE)
		{
			m_vecWayPoint.push_back(Vec3(746.f, 15.f, 190.f));
			m_vecWayPoint.push_back(Vec3(1275.f, 15.f, 182.f));
			m_vecWayPoint.push_back(Vec3(1864.f, 15.f, 371.f));
			m_vecWayPoint.push_back(Vec3(2005.f, 15.f, 1434.f));
			m_vecWayPoint.push_back(Vec3(2000.f, 15.f, 1888.f));
		}
		else if (m_eFaction == Faction::RED)
		{
			m_vecWayPoint.push_back(Vec3(2009.f, 15.f, 1444.f));
			m_vecWayPoint.push_back(Vec3(2001.f, 15.f, 1036.f));
			m_vecWayPoint.push_back(Vec3(1849.f, 15.f, 357.f));
			m_vecWayPoint.push_back(Vec3(747.f, 15.f, 183.f));
			m_vecWayPoint.push_back(Vec3(288.f, 15.f, 186.f));
		}
	}
	break;
	}


	// FSM에 State 추가
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) == nullptr)	GetOwner()->AddComponent(new CCollider2D);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER3D) == nullptr) GetOwner()->AddComponent(new CCollider3D);

	GetOwner()->Fsm()->AddState(L"Walk", new CMinionWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CMinionDeathState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMinionAttackState);
	GetOwner()->Fsm()->AddState(L"Chase", new CMinionChaseState);

	GetOwner()->Fsm()->ChangeState(L"Walk");

	// Skill에 BasicAttack 추가
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);

	// 사거리 적용
	CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
	if (AttackRange == nullptr) {
		AttackRange = new CGameObject;
		AttackRange->AddComponent(new CTransform);
		AttackRange->AddComponent(new CCollider2D);
		AttackRange->AddComponent(new CAttackRangeScript);

		AttackRange->SetName(L"AttackRange");
		AttackRange->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		AttackRange->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		AttackRange->Collider2D()->SetAbsolute(true);
		AttackRange->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
		GetOwner()->AddChild(AttackRange);
		AttackRange->ChangeLayer(L"AttackRange");
	}
	AttackRange->Collider2D()->SetOffsetScale(Vec2(m_fAttackRange, m_fAttackRange));

	m_fHP = m_fMaxHP;

}

void CMinionScript::tick()
{
	if (m_bUnitDead)
		return;

	if (m_fHP <= 0)
	{
		//죽음
		m_bUnitDead = true;
		GetOwner()->Fsm()->ChangeState(L"Death");
		return;
	}

	CUnitScript::tick();

	// 공속 쿨타임 세줌
	AttackCoolDown();

	// 타겟 찾기
	if (!m_pTarget)
		FindTarget();


	/*
	1. 라인별 타겟 포인트로 전진한다.
	2. 타겟포인트에 도착하면, 벡터의 인덱스를 증가시켜 다음 타겟포인트로 전진한다.
	3. 전진 도중 사거리 내에 공격할 수 있는 몹을 만나면, 공격 상태로 전환한다.
	4. 타겟 오브젝트가 죽었다면, 다시 Walk 상태로 전환한다.
	5. 타겟 오브젝트가 죽지 않았지만 사거리 내에 없는 경우, 2초간 타겟을 따라간다.
	6. 다시 사거리 내에 공격할 오브젝트가 없다면, Walk 상태로 전환되어 전진을 계속한다.
	*/
}

void CMinionScript::Move()
{
	// 움직일 수 없는 상황인 경우 return
	if ((m_eRestraint & RESTRAINT::CAN_MOVE) == 0)
		return;

	// 이동
	if (PathFindMove(m_fMoveSpeed, true))
	{

	}
	else
	{
		// 갈 수 없는 경로 반환 시 -> 목표 지점에 다 왔다는 뜻

		// 재검증 후, 목표지점 도착 확인 시 WayPointIdx++
		if (IsAtWayPoint())
		{
			if (m_iWayPointIdx < m_vecWayPoint.size() - 1)
				m_iWayPointIdx++;
		}
		GetOwner()->PathFinder()->FindPath(m_vecWayPoint[m_iWayPointIdx]);
	}
}

void CMinionScript::FindTarget()
{
	CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
	vector<CGameObject*> UnitsInRange = AttackRange->GetScript<CAttackRangeScript>()->GetUnitsInRange();
	if (UnitsInRange.size() > 0)
	{
		m_pTarget = nullptr;

		for (int i = 0; i < UnitsInRange.size(); i++)
		{
			if (!UnitsInRange[i]->GetScript<CUnitScript>()->IsUnitDead())
			{
				m_pTarget = UnitsInRange[i];
				return;
			}
		}
	}
	else
		m_pTarget = nullptr;
}

void CMinionScript::AttackCoolDown()
{
	m_fCurAttackCoolTime += DT;
	if (m_fCurAttackCoolTime >= m_fAttackSpeed)
	{
		m_fCurAttackCoolTime = m_fAttackSpeed;
	}
}

bool CMinionScript::IsTargetValid(CGameObject* _Obj)
{
	// 타겟이 죽어서 없어졌다면
	if (_Obj == nullptr || _Obj->IsDead())
	{
		m_pTarget = nullptr;
		return false;
	}
	if (_Obj->GetScript<CUnitScript>()->IsUnitDead())
	{
		m_pTarget = nullptr;
		return false;
	}

	return true;
}

bool CMinionScript::IsTargetInRange(CGameObject* _Obj)
{
	if (!IsTargetValid(_Obj))
		return false;

	CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
	vector<CGameObject*> UnitsInRange = AttackRange->GetScript<CAttackRangeScript>()->GetUnitsInRange();

	auto it = find(UnitsInRange.begin(), UnitsInRange.end(), _Obj);

	// 사거리 내부에 더이상 그 오브젝트가 없다면
	if (it == UnitsInRange.end())
		return false;

	else 
	{
		if ((*it)->GetScript<CUnitScript>()->IsUnitDead())
			return false;
		else
			return true;
	}
}

bool CMinionScript::IsAtWayPoint()
{
	Vec3  MinionPos = GetOwner()->Transform()->GetRelativePos();
	Vec3  WayPoint = m_vecWayPoint[m_iWayPointIdx];

	float distance = sqrt(pow(WayPoint.x - MinionPos.x, 2) + pow(WayPoint.z - MinionPos.z, 2));

	if (distance <= 10.5f)
		return true;
	else
		return false;
}
