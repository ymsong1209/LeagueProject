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
	, m_eLane(Lane::NONE)
	, m_vecWayPoint{}
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_eMinionType, "Type");
}

CMinionScript::~CMinionScript()
{
}

void CMinionScript::begin()
{
	m_fHP = m_fMaxHP;
	GetOwner()->Transform()->SetUseMouseOutline(true);
	
	// 진영
	//SetFaction(Faction::RED);

	// 미니언 타입 별 정보 세팅
	switch (m_eMinionType)
	{
	case MinionType::MELEE:
	{
		m_fAttackPower = 1.f;
		m_fAttackRange = 50.f;
		m_fAttackSpeed = 2.f;
		m_fMaxHP = 50.f;
	}
	break;
	case MinionType::RANGED:
		break;
	case MinionType::SEIGE:
		break;
	case MinionType::SUPER:
		break;
	}

	// 본인의 Lane, MinionType에 따라 정보 변경
	switch (m_eLane)
	{
	case Lane::TOP:
	{
		// Top 동선에서 지나야할 곳 m_vecWayPoint로 pushback
	}
		break;
	case Lane::MID:
	{
		// Mid 동선에서 지나야할 곳 m_vecWayPoint로 pushback
	}
		break;
	case Lane::BOTTOM:
	{
		// Bot 동선에서 지나야할 곳 m_vecWayPoint로 pushback
	}
		break;
	}


	// FSM에 State 추가
	GetOwner()->Fsm()->AddState(L"Walk", new CMinionWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CMinionDeathState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMinionAttackState);
	GetOwner()->Fsm()->AddState(L"Chase", new CMinionAttackState);

	GetOwner()->Fsm()->ChangeState(L"Walk");

	// Skill에 BasicAttack 추가
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);

}

void CMinionScript::tick() 
{
	CUnitScript::tick();

	// 공속 쿨타임 세줌
	AttackCoolDown();

	// 타겟 지정
	SelectTarget();


	/*
	1. 라인별 타겟 포인트로 전진한다.
	2. 타겟포인트에 도착하면, 벡터의 인덱스를 증가시켜 다음 타겟포인트로 전진한다.
	3. 전진 도중 사거리 내에 공격할 수 있는 몹을 만나면, 공격 상태로 전환한다.
	4. 사거리 내의 적 중 우선순위를 따져 타겟 오브젝트를 결정한다.
	5. 타겟 오브젝트가 더 이상 사거리 내에 없거나 죽었다면, 타겟 오브젝트를 다시 결정한다.
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
			m_iWayPointIdx++;
	}
}

void CMinionScript::SelectTarget()
{
	// 공격 타겟 설정
	if (!m_pTarget)
	{
		CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
		vector<CGameObject*> UnitsInRange = AttackRange->GetScript<CAttackRangeScript>()->GetUnitsInRange();
		if (UnitsInRange.size() > 0)
			m_pTarget = UnitsInRange[0];
	}
	else
	{
		// 타겟이 있는 경우

	}
}

void CMinionScript::AttackCoolDown()
{
	if (!CanAttack())
	{
		m_curAttackCoolTime += DT;
		if (m_curAttackCoolTime >= m_fAttackSpeed)
			m_curAttackCoolTime = m_fAttackSpeed;
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
	if (!(_Obj->GetScript<CUnitScript>()->IsUnitDead()))
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
		return true;
}

bool CMinionScript::IsAtWayPoint()
{
	Vec3  MinionPos = GetOwner()->Transform()->GetRelativePos();
	Vec3  WayPoint = m_vecWayPoint[m_iWayPointIdx];

	float distance = sqrt(pow(WayPoint.x - MinionPos.x, 2) + pow(WayPoint.z - MinionPos.z, 2));
	
	if (distance <= 1.f)
		return true;
	else
		return false;
}
