#include "pch.h"
#include "CJinxScript.h"

#include "CJinxIdleState.h"
#include "CJinxWalkState.h"
#include "CJinxDeathState.h"
#include "CJinxRespawnState.h"
#include "CJinxAttackState.h"


#include "CBaseAttack.h"
#include "CJinxQ.h"
#include "CJinxw.h"

CJinxScript::CJinxScript()
	:CChampionScript((UINT)JINXSCRIPT)
{
	m_fMaxHP = 5;
	m_fAttackPower = 5.f;
}

CJinxScript::~CJinxScript()
{
}

void CJinxScript::begin()
{
	// FSM에 State 추가
	GetOwner()->Fsm()->AddState(L"Idle", new CJinxIdleState);
	GetOwner()->Fsm()->AddState(L"Walk", new CJinxWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CJinxDeathState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CJinxRespawnState);
	GetOwner()->Fsm()->AddState(L"Attack", new CJinxAttackState);

	// Skill에 Jinx Skill 추가
	m_Skill[0] = new CBaseAttack;
	m_Skill[0]->SetOwnerScript(this);
	m_Skill[1] = new CJinxQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CJinxW;
	m_Skill[2]->SetOwnerScript(this);

	// Spell에 소환사 주문 추가


	// 진영
	// SetFaction(Faction::BLUE);

	CChampionScript::begin();
}

void CJinxScript::tick()
{
	CChampionScript::tick();

	// 징크스의 공격력 / 사거리 / 공격속도는 Q스킬의 추가 공격력을 포함한다.


	//CJinxQ* JinxQ = dynamic_cast<CJinxQ*>(m_Skill[0]);
	//if (JinxQ != nullptr)
	//{
	//	m_fAttackPower += JinxQ->GetSkillDamage();
	//	m_fAttackSpeed += JinxQ->GetSkillAttackSpeed();
	//	m_fAttackRange += JinxQ->GetSkillAttackRange();
	//}
}
