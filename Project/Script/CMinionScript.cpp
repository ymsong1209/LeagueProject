#include "pch.h"
#include "CMinionScript.h"

CMinionScript::CMinionScript()
	:CMobScript((UINT)SCRIPT_TYPE::MINIONSCRIPT)
	, m_eLane(Lane::NONE)
	, m_vecTargetPoint{}
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_eMinionType, "Type");
}

CMinionScript::~CMinionScript()
{
}

void CMinionScript::begin()
{

	GetOwner()->Transform()->SetUseMouseOutline(true);
	// 진영
	SetFaction(Faction::RED);

	// 본인의 Lane, MinionType에 따라 정보 변경

	switch (m_eLane)
	{
	case Lane::TOP:
	{
		// Top 동선에서 지나야할 곳 m_vecTargetPoint로 pushback
	}
		break;
	case Lane::MID:
	{
		// Mid 동선에서 지나야할 곳 m_vecTargetPoint로 pushback
	}
		break;
	case Lane::BOTTOM:
	{
		// Bot 동선에서 지나야할 곳 m_vecTargetPoint로 pushback
	}
		break;
	}

	switch (m_eMinionType)
	{
	case MinionType::MELEE:
	{
		m_fAttackPower = 1.f;
		m_fAttackRange = 50.f;
		m_fAttackSpeed = 1.f;
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

	m_fHP = m_fMaxHP;
}

void CMinionScript::tick() 
{
	/*
	1. 라인별 타겟 포인트로 전진한다.
	2. 타겟포인트에 도착하면, 벡터의 인덱스를 증가시켜 다음 타겟포인트로 전진한다.
	3. 전진 도중 사거리 내에 공격할 수 있는 몹을 만나면, 공격 상태로 전환한다.
	4. 사거리 내의 적 중 우선순위를 따져 타겟 오브젝트를 결정한다.
	5. 타겟 오브젝트가 더 이상 사거리 내에 없거나 죽었다면, 타겟 오브젝트를 다시 결정한다.
	6. 다시 사거리 내에 공격할 오브젝트가 없다면, Walk 상태로 전환되어 전진을 계속한다.
	*/
}
