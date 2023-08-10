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
	// FSM�� State �߰�
	GetOwner()->Fsm()->AddState(L"Idle", new CJinxIdleState);
	GetOwner()->Fsm()->AddState(L"Walk", new CJinxWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CJinxDeathState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CJinxRespawnState);
	GetOwner()->Fsm()->AddState(L"Attack", new CJinxAttackState);
	GetOwner()->Fsm()->AddState(L"Q", new CJinxQState);
	GetOwner()->Fsm()->AddState(L"W", new CJinxWState);
	GetOwner()->Fsm()->AddState(L"E", new CJinxEState);
	GetOwner()->Fsm()->AddState(L"R", new CJinxRState);

	// Skill�� Jinx Skill �߰�
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

	// Spell�� ��ȯ�� �ֹ� �߰�

	CChampionScript::begin();
}

void CJinxScript::tick()
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
