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
	
	// ����
	//SetFaction(Faction::RED);

	// �̴Ͼ� Ÿ�� �� ���� ����
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

	// ������ Lane, MinionType�� ���� ���� ����
	switch (m_eLane)
	{
	case Lane::TOP:
	{
		// Top �������� �������� �� m_vecWayPoint�� pushback
	}
		break;
	case Lane::MID:
	{
		// Mid �������� �������� �� m_vecWayPoint�� pushback
	}
		break;
	case Lane::BOTTOM:
	{
		// Bot �������� �������� �� m_vecWayPoint�� pushback
	}
		break;
	}


	// FSM�� State �߰�
	GetOwner()->Fsm()->AddState(L"Walk", new CMinionWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CMinionDeathState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMinionAttackState);
	GetOwner()->Fsm()->AddState(L"Chase", new CMinionAttackState);

	GetOwner()->Fsm()->ChangeState(L"Walk");

	// Skill�� BasicAttack �߰�
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);

}

void CMinionScript::tick() 
{
	CUnitScript::tick();

	// ���� ��Ÿ�� ����
	AttackCoolDown();

	// Ÿ�� ����
	SelectTarget();


	/*
	1. ���κ� Ÿ�� ����Ʈ�� �����Ѵ�.
	2. Ÿ������Ʈ�� �����ϸ�, ������ �ε����� �������� ���� Ÿ������Ʈ�� �����Ѵ�.
	3. ���� ���� ��Ÿ� ���� ������ �� �ִ� ���� ������, ���� ���·� ��ȯ�Ѵ�.
	4. ��Ÿ� ���� �� �� �켱������ ���� Ÿ�� ������Ʈ�� �����Ѵ�.
	5. Ÿ�� ������Ʈ�� �� �̻� ��Ÿ� ���� ���ų� �׾��ٸ�, Ÿ�� ������Ʈ�� �ٽ� �����Ѵ�.
	6. �ٽ� ��Ÿ� ���� ������ ������Ʈ�� ���ٸ�, Walk ���·� ��ȯ�Ǿ� ������ ����Ѵ�.
	*/
}

void CMinionScript::Move()
{
	// ������ �� ���� ��Ȳ�� ��� return
	if ((m_eRestraint & RESTRAINT::CAN_MOVE) == 0)
		return;

	// �̵�
	if (PathFindMove(m_fMoveSpeed, true))
	{

	}
	else
	{
		// �� �� ���� ��� ��ȯ �� -> ��ǥ ������ �� �Դٴ� ��

		// ����� ��, ��ǥ���� ���� Ȯ�� �� WayPointIdx++
		if (IsAtWayPoint())
			m_iWayPointIdx++;
	}
}

void CMinionScript::SelectTarget()
{
	// ���� Ÿ�� ����
	if (!m_pTarget)
	{
		CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
		vector<CGameObject*> UnitsInRange = AttackRange->GetScript<CAttackRangeScript>()->GetUnitsInRange();
		if (UnitsInRange.size() > 0)
			m_pTarget = UnitsInRange[0];
	}
	else
	{
		// Ÿ���� �ִ� ���

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
	// Ÿ���� �׾ �������ٸ�
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

	// ��Ÿ� ���ο� ���̻� �� ������Ʈ�� ���ٸ�
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
