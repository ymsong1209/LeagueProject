#include "pch.h"
#include "CJinxScript.h"

#include "CJinxIdleState.h"
#include "CJinxWalkState.h"
#include "CJinxDeathState.h"
#include "CJinxRespawnState.h"

#include "CJinxQ.h"
#include "CJinxw.h"

CJinxScript::CJinxScript()
	:CChampionScript((UINT)JINXSCRIPT)
{
	m_fMaxHP = 5;
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

	// Skill�� Jinx Skill �߰�
	m_Skill[1] = new CJinxQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CJinxW;
	m_Skill[2]->SetOwnerScript(this);

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
