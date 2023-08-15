#include "pch.h"
#include "CChampionScript.h"
#include  <algorithm>

#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>

#include "CGameEvent.h"
#include "CGameEventMgr.h"

#include "CSkill.h"
#include "CAttackRangeScript.h"
#include "CSendServerEventMgr.h"

#define MAX_SKILL_LEVEL 5
#define MAX_ULT_LEVEL 3

CChampionScript::CChampionScript(UINT ScriptType)
	: CUnitScript(ScriptType)
	, m_fExp(0)
	, m_EquippedSpell{}
	, m_fRespawnTime(5)
	, m_bIsAttackingChampion(false)
{
	m_eUnitType = UnitType::CHAMPION;

	m_iDropGold = 300;
	m_fDropExp = 30.f;

	m_fMaxExp = 50.f;
}

CChampionScript::CChampionScript()
	: CUnitScript((UINT)SCRIPT_TYPE::CHAMPIONSCRIPT)
{
}

CChampionScript::~CChampionScript()
{
	
}

void CChampionScript::begin()
{
	CUnitScript::begin();

	// FSM
	if (GetOwner()->Fsm() == nullptr)
		return;
	GetOwner()->Fsm()->ChangeState(L"Idle");

	// 스킬 레벨 초기화
	m_SkillLevel[0] = 1;
	m_SkillLevel[1] = 1;
	m_SkillLevel[2] = 1;
	m_SkillLevel[3] = 1;
	m_SkillLevel[4] = 1;
	/// 소환사 주문 배열에 넣어주기
}

void CChampionScript::tick()
{
	CUnitScript::tick();

	if (KEY_TAP(KEY::P))
	{
		m_fHP = 0;
	}

	CheckSkills();
	if (CheckDeath())
		return;
	
	GetInput();
	CheckStatus();
	Move();

}

void CChampionScript::BeginOverlap(CCollider2D* _Other)
{
	// 사거리와 부딪친게 아니라면 return
	if (_Other->GetOwner()->GetScript<CAttackRangeScript>() == nullptr) return;

	// 부모가 포탑이며, 진영이 다른 포탑일 경우
	if (_Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetUnitType() == UnitType::TURRET
		&& _Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetFaction() != GetFaction())
	{
		m_bIsInsideEnemyTurretRange = true;
	}
}

void CChampionScript::EndOverlap(CCollider2D* _Other)
{
	// 사거리와 부딪친게 아니라면 return
	if (_Other->GetOwner()->GetScript<CAttackRangeScript>() == nullptr) return;

	// 부모가 포탑이며, 진영이 다른 포탑일 경우
	if (_Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetUnitType() == UnitType::TURRET
		&& _Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetFaction() != GetFaction())
	{
		m_bIsInsideEnemyTurretRange = false;
		m_bIsAttackingChampion = false;
	}
}

bool CChampionScript::CheckDeath()
{
	// 죽음
	if (m_fHP <= 0)
	{
		// 죽음 이벤트
		DeathEvent* evn = dynamic_cast<DeathEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_DEATH));
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
		
		// 죽음 체크
		m_bUnitDead = true;

		// 아무것도 못하는 상태
		m_eRestraint |= RESTRAINT::BLOCK;

		m_fRespawnTime -= EditorDT;
		// 부활 대기시간 끝나면
		if (m_fRespawnTime <= 0)
		{
			m_fHP = m_fMaxHP;
			m_bUnitDead = false;
			m_fRespawnTime = 5;
			m_eCurCC = CC::NO_CC;
			m_eRestraint = RESTRAINT::NO_RESTRAINT;

			// 길찾기 컴포넌트에 남은 경로값이 있다면 Clear
			PathFinder()->ClearPath();

			// 우물로 리스폰
			if(m_eFaction == Faction::BLUE)
				GetOwner()->Transform()->SetRelativePos(100.f, 30.f, 100.f);
			else if(m_eFaction == Faction::RED)
				GetOwner()->Transform()->SetRelativePos(2083.0f, 30.0f, 2137.0f);

			// 부활 이벤트
			RespawnEvent* evn = dynamic_cast<RespawnEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_RESPAWN));
			if (evn != nullptr)
			{
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}

			return false;
		}
		return true;
	}
	return false;

}

void CChampionScript::CheckStatus()
{
	m_fHP += 2.0f * DT;
	m_fMP += 5.0f * DT;

	m_fHP = clamp(m_fHP, 0.f, m_fMaxHP);
	m_fMP = clamp(m_fMP, 0.f, m_fMaxMP);
	
	if (m_fExp >= m_fMaxExp)
	{
		LevelUp();
	}

}

void CChampionScript::GetInput()
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

		if (m_Skill[1]->CSkill::Use())
		{
			// Q이벤트 발생
			PlayerQEvent* evn = dynamic_cast<PlayerQEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_Q));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
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
		m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_2))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[2] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[2] += 1;
		m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_3))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[3] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[3] += 1;
		m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_4))
	{
		// 스킬 포인트가 없거나 이미 스킬 레벨이 Max인 경우 return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[4] >= MAX_ULT_LEVEL)
			return;

		m_SkillLevel[4] += 1;
		m_iSkillLevelUpPoint -= 1;
	}


	// 소환사 주문

}
void CChampionScript::CheckSkills()
{
	
	for (int i = 0; i < 5; i++)
	{
		if (m_Skill[i] == nullptr)
			continue;

		m_Skill[i]->tick();
	}
}

void CChampionScript::Move()
{
	// 움직일 수 없는 상황인 경우 return
	if ((m_eRestraint & RESTRAINT::CANNOT_MOVE) != 0)
		return;

	// 이동
	if (PathFindMove(m_fMoveSpeed, true))
	{
		// 이동 이벤트
		MoveEvent* evn = dynamic_cast<MoveEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_MOVE));
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
	else
	{
		// 이동 벡터값이 NaN -> 이동 불가, 멈춤
		StopEvent* evn = dynamic_cast<StopEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_STOP));;
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
}

void CChampionScript::LevelUp()
{
	m_iLevel++;

	//레벨업 이펙트 및 파티클

	// 초과된 만큼 exp를 남기고 스킬 포인트 추가
	m_fExp = m_fExp - m_fMaxExp;
	m_iSkillLevelUpPoint++;
}


