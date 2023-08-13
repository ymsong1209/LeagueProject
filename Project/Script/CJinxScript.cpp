#include "pch.h"
#include "CJinxScript.h"

#include "CJinxIdleState.h"
#include "CJinxWalkState.h"
#include "CJinxDeathState.h"
#include "CJinxRespawnState.h"
#include "CJinxAttackState.h"

#include "CBasicAttack.h"
#include "CJinxQState.h"
#include "CJinxWState.h"
#include "CJinxEState.h"
#include "CJinxRState.h"

#include "CJinxQ.h"
#include "CJinxw.h"
#include "CJinxE.h"
#include "CJinxR.h"

CJinxScript::CJinxScript()
	:CChampionScript((UINT)JINXSCRIPT)
{
	m_fMaxHP = 50;
	m_fMaxMP = 300;
	//m_fAttackPower = 5.f;
	m_fAttackPower = 10.f;
	m_fMoveSpeed = 100.f;
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
	GetOwner()->Fsm()->AddState(L"Q", new CJinxQState);
	GetOwner()->Fsm()->AddState(L"W", new CJinxWState);
	GetOwner()->Fsm()->AddState(L"E", new CJinxEState);
	GetOwner()->Fsm()->AddState(L"R", new CJinxRState);

	// Skill에 Jinx Skill 추가
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	m_Skill[1] = new CJinxQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CJinxW;
	m_Skill[2]->SetOwnerScript(this);
	m_Skill[3] = new CJinxE;
	m_Skill[3]->SetOwnerScript(this);
	m_Skill[4] = new CJinxR;
	m_Skill[4]->SetOwnerScript(this);

	// Spell에 소환사 주문 추가

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
