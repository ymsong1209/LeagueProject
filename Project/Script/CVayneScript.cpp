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
	// FSM�� State �߰�
	GetOwner()->Fsm()->AddState(L"Idle", new CVayneIdleState);
	GetOwner()->Fsm()->AddState(L"Walk", new CVayneWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CVayneDeathState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CVayneRespawnState);

	// Skill�� Jinx Skill �߰�
	m_Skill[1] = new CVayneQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CVayneW;
	m_Skill[2]->SetOwnerScript(this);

	// Spell�� ��ȯ�� �ֹ� �߰�

	CChampionScript::begin();
}

void CVayneScript::tick()
{
	CChampionScript::tick();

	// ¡ũ���� ���ݷ� / ��Ÿ� / ���ݼӵ��� Q��ų�� �߰� ���ݷ��� �����Ѵ�.


	//CJinxQ* JinxQ = dynamic_cast<CJinxQ*>(m_Skill[0]);
	//if (JinxQ != nullptr)
	//{
	//	m_fAttackPower += JinxQ->GetSkillDamage();
	//	m_fAttackSpeed += JinxQ->GetSkillAttackSpeed();
	//	m_fAttackRange += JinxQ->GetSkillAttackRange();
	//}
}




 
 

 
