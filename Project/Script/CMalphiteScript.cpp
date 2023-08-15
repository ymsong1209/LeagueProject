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
#include "CMalphiteBasicAttack.h"
#include "CMalphiteQ.h"
#include "CMalphiteW.h"
#include "CMalphiteE.h"
#include "CMalphiteR.h"

#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>
#include "CAttackRangeScript.h"

#define MAX_SKILL_LEVEL 5
#define MAX_ULT_LEVEL 3

CMalphiteScript::CMalphiteScript()
	:CChampionScript((UINT)MALPHITESCRIPT)
{
	m_fMaxHP = 550;
	m_fMaxMP = 50;

	//m_fAttackPower = 5.f;
	m_fAttackPower = 150.f;
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
	m_Skill[0] = new CMalphiteBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	m_Skill[1] = new CMalphiteQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CMalphiteW;
	m_Skill[2]->SetOwnerScript(this);
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
			UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
			
			GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MAT_malphite.mtrl"), 0);
			CSendServerEventMgr::GetInst()->SendSetMtrlPacket(targetId, 0, L"material\\MAT_malphite.mtrl");
		}
	}
	
}

void CMalphiteScript::GetInput()
{
	// 들어온 입력을 확인합니다.

	if (KEY_TAP(KEY::RBTN))
	{
		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = MainCam->GetRay();

		// 현재 마우스 ray와 충돌 중인 오브젝트가 있는지 확인
		vector<CGameObject*>& MouseOverlapObj = MainCam->GetMouseOverlapObj();
		if (MouseOverlapObj.size() >= 1)
		{
			CGameObject* Unit = MouseOverlapObj[0];
			if (Unit == this->GetOwner()) return;
			CUnitScript* UnitScript = Unit->GetScript<CUnitScript>();

			// 오브젝트가 현재 챔피언의 사거리 내에 있는지 확인
			CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
			CAttackRangeScript* AttackRangeScript = AttackRange->GetScript<CAttackRangeScript>();
			vector<CGameObject*> UnitinRange = AttackRangeScript->GetUnitsInRange();

			auto it = find(UnitinRange.begin(), UnitinRange.end(), Unit);

			// 사거리 내에 있음
			if (it != UnitinRange.end())
			{
				// 해당 유닛이 죽었다면 return
				if (UnitScript->IsUnitDead())
					return;

				// 공격 이벤트 발생
				BasicAttackEvent* evn = dynamic_cast<BasicAttackEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK));
				if (evn != nullptr)
				{
					evn->Clear();
					evn->SetUserObj(GetOwner());
					evn->SetTargetObj(UnitScript->GetOwner());

					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}


			}

			// 사거리 내에 없음
			else
			{
				// 사거리 내에 들어올 때까지 이동
				// 대신 공격은 사거리 내에 있을때 다시 눌러야함
				if ((m_eRestraint & RESTRAINT::CANNOT_MOVE) != 0)
					return;

				CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"LoLMapCollider");
				IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
				Vec3 TargetPos = result.vCrossPoint;	// 클릭 좌표
				PathFinder()->FindPath(TargetPos);
			}
		}
		else
		{
			// 그 외(땅을 클릭한 경우)

			// 움직일 수 없는 상황인 경우 return
			if ((m_eRestraint & RESTRAINT::CANNOT_MOVE) != 0)
				return;

			CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"LoLMapCollider");
			IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
			Vec3 TargetPos = result.vCrossPoint;	// 클릭 좌표
			PathFinder()->FindPath(TargetPos);
		}
	}

	if (KEY_TAP(KEY::Q))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[1]->GetCost() > m_fMP)
			return;


		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = MainCam->GetRay();

		// 현재 마우스 ray와 충돌 중인 오브젝트가 있는지 확인
		vector<CGameObject*>& MouseOverlapObj = MainCam->GetMouseOverlapObj();
		if (MouseOverlapObj.size() >= 1)
		{
			CGameObject* Unit = MouseOverlapObj[0];
			if (Unit == this->GetOwner()) return;

			CUnitScript* UnitScript = Unit->GetScript<CUnitScript>();

			// 해당 유닛이 죽었다면 return
			if (UnitScript->IsUnitDead())
				return;

			if (UnitScript->GetFaction() == GetFaction()) return;

			if (m_Skill[1]->CSkill::Use())
			{
				// Q이벤트 발생
				PlayerQEvent* evn = dynamic_cast<PlayerQEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_Q));
				if (evn != nullptr)
				{
					evn->Clear();
					evn->SetUserObj(GetOwner());
					evn->SetTargetObj(Unit);
					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}
			}
		}


		
	}
	if (KEY_TAP(KEY::W))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[2]->GetCost() > m_fMP)
			return;

		if (m_Skill[2]->CSkill::Use())
		{
			// W 이벤트 발생
			PlayerWEvent* evn = dynamic_cast<PlayerWEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_W));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}
	if (KEY_TAP(KEY::E))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[3]->GetCost() > m_fMP)
			return;

		if (m_Skill[3]->CSkill::Use())
		{
			// E 이벤트 발생
			PlayerEEvent* evn = dynamic_cast<PlayerEEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_E));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}
	if (KEY_TAP(KEY::R))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[4]->GetCost() > m_fMP)
			return;

		if (m_Skill[4]->CSkill::Use())
		{
			// R 이벤트 발생
			PlayerREvent* evn = dynamic_cast<PlayerREvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_R));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}

	if (KEY_TAP(KEY::_1))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[1] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[1] += 1;
		//m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_2))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[2] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[2] += 1;
		//m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_3))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[3] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[3] += 1;
		//m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_4))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[4] >= MAX_ULT_LEVEL)
			return;

		m_SkillLevel[4] += 1;
		//m_iSkillLevelUpPoint -= 1;
	}

	// 소환사 주문
}
