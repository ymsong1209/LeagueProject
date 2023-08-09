#include "pch.h"
#include "CMalphiteScript.h"

#include "CMalphiteIdleState.h"
#include "CMalphiteWalkState.h"
#include "CMalphiteDeathState.h"
#include "CMalphiteRespawnState.h"
#include "CMalphiteAttackState.h"
#include "CMalphiteQState.h"
#include "CMalphiteWState.h"
#include "CMalphiteEState.h"
#include "CMalphiteRState.h"

#include "CBasicAttack.h"
#include "CMalphiteQ.h"

#include "CMalphiteE.h"
#include "CMalphiteR.h"


CMalphiteScript::CMalphiteScript()
	:CChampionScript((UINT)MALPHITESCRIPT)
{
	m_fMaxHP = 50;
	m_fMaxMP = 50;
	m_fHP = 50;
	m_fMP = 50;
	//m_fAttackPower = 5.f;
	m_fAttackPower = 10.f;
	m_fMoveSpeed = 100.f;
}

CMalphiteScript::~CMalphiteScript()
{
}

void CMalphiteScript::begin()
{
	// FSM에 State 추가
	GetOwner()->Fsm()->AddState(L"Idle", new CMalphiteIdleState);
	GetOwner()->Fsm()->AddState(L"Walk", new CMalphiteWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CMalphiteDeathState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CMalphiteRespawnState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMalphiteAttackState);
	GetOwner()->Fsm()->AddState(L"Recall", new CMalphiteRespawnState);
	GetOwner()->Fsm()->AddState(L"Q", new CMalphiteQState);
	GetOwner()->Fsm()->AddState(L"W", new CMalphiteWState);
	GetOwner()->Fsm()->AddState(L"E", new CMalphiteEState);
	GetOwner()->Fsm()->AddState(L"R", new CMalphiteRState);

	// Skill에 Jinx Skill 추가
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	m_Skill[1] = new CMalphiteQ;
	m_Skill[1]->SetOwnerScript(this);
	//m_Skill[2] = new CMalphiteE;
	//m_Skill[2]->SetOwnerScript(this);
	m_Skill[3] = new CMalphiteE;
	m_Skill[3]->SetOwnerScript(this);
	m_Skill[4] = new CMalphiteR;
	m_Skill[4]->SetOwnerScript(this);

	// Spell에 소환사 주문 추가


	CChampionScript::begin();
}

void CMalphiteScript::tick()
{
	CChampionScript::tick();

	m_fAttackAnimCoolTime += DT;
	if (m_bWActive) {
		m_fWActiveTime += DT;
		if (m_fWActiveTime > 4.f) {
			m_bWActive = false;
			m_fWActiveTime = 0.f;

			m_fAttackPower = 10.f;

			//재질을 바꿔줌

		}
	}
	
}
