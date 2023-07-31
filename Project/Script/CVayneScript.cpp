#include "pch.h"
#include "CVayneScript.h"

//#include "CJinxIdleState.h"
//#include "CJinxWalkState.h"
//#include "CJinxDeathState.h"
//#include "CJinxRespawnState.h"
//
//#include "CJinxQ.h"
//#include "CJinxw.h"

#include "CVayneIdleState.h"
#include "CVayneWalkState.h"
#include "CVayneDeathState.h"
#include "CVayneRespawnState.h"

#include "CVayneQ.h"
#include "CVayneW.h"



CVayneScript::CVayneScript()
	:CChampionScript((UINT)VAYNESCRIPT)
{
	m_fMaxHP = 5;
	m_fHP = 5;
}

CVayneScript::~CVayneScript()
{
}

void CVayneScript::begin()
{
	// FSM에 State 추가
	GetOwner()->Fsm()->AddState(L"Idle", new CVayneIdleState);
	GetOwner()->Fsm()->AddState(L"Walk", new CVayneWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CVayneDeathState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CVayneRespawnState);

	// Skill에 Jinx Skill 추가
	m_Skill[1] = new CVayneQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CVayneW;
	m_Skill[2]->SetOwnerScript(this);

	// Spell에 소환사 주문 추가

	CChampionScript::begin();
}

void CVayneScript::tick()
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




 
 

 
