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

CChampionScript::CChampionScript(UINT ScriptType)
	: CUnitScript(ScriptType)
	, m_iLevel(1)
	, m_fExp(0)
	, m_EquippedSpell{}
	, m_fRespawnTime(5)
	, m_eCurCC(CC::CLEAR)
	, m_eRestraint(RESTRAINT::DEFAULT)
	, m_Skill{}
{
	m_eUnitType = UnitType::CHAMPION;

	// test	
	m_fMaxHP = 5;
}

CChampionScript::CChampionScript()
	: CUnitScript((UINT)SCRIPT_TYPE::CHAMPIONSCRIPT)
{
}

CChampionScript::~CChampionScript()
{
	Safe_Del_Array(m_Skill);
}

void CChampionScript::begin()
{
	CUnitScript::begin();

	// 소환사 주문 배열에 넣어주기
}

void CChampionScript::tick()
{
	if (KEY_TAP(KEY::P))
	{
		m_fHP = 0;
	}

	if (CheckDeath())
		return;

	GetInput();
	CheckStatus();
	Move();
	CheckSkills();

}

void CChampionScript::BeginOverlap(CCollider2D* _Other)
{
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

		// 아무것도 못하는 상태
		m_eRestraint = BLOCK;

		m_fRespawnTime -= DT;
		// 부활 대기시간 끝나면
		if (m_fRespawnTime <= 0)
		{
			m_fHP = m_fMaxHP;
			m_fRespawnTime = 5;
			m_eRestraint = DEFAULT;

			// 길찾기 컴포넌트에 남은 경로값이 있다면 Clear
			PathFinder()->ClearPath();

			// 우물로 리스폰
			GetOwner()->Transform()->SetRelativePos(100.f, 30.f, 100.f);

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
	// 체력 / 마나 리젠 예시
	m_fHP += 2.0f * DT;
	m_fMP += 5.0f * DT;

	if (m_fHP > m_fMaxHP)
		m_fHP = m_fMaxHP;

	if (m_fMP > m_fMaxMP)
		m_fMP = m_fMaxMP;
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
			CUnitScript* UnitScript = Unit->GetScript<CUnitScript>();

			// 오브젝트가 현재 챔피언의 사거리 내에 있는지 확인
			CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
			CAttackRangeScript* AttackRangeScript = AttackRange->GetScript<CAttackRangeScript>();
			vector<CGameObject*> UnitinRange = AttackRangeScript->GetUnitsInRange();

			auto it = find(UnitinRange.begin(), UnitinRange.end(), Unit);

			// 사거리 내에 있음
			if (it != UnitinRange.end())
			{
				// 공격 이벤트 발생
				BaseAttackEvent* evn = dynamic_cast<BaseAttackEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_BASE_ATTACK));
				if (evn != nullptr)
				{
					evn->Clear();
					evn->SetUserID(GetOwner()->GetID());
					evn->SetTargetID(UnitScript->GetOwner()->GetID());

					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}

				// 서버에게 보낼 이벤트
				SkillInfo* skillInfo = new SkillInfo;
				skillInfo->OwnerId = GetServerID();
				skillInfo->TargetId = UnitScript->GetServerID();
				skillInfo->SkillLevel = 1;
				skillInfo->skillType = SkillType::BASIC_ATTACK;

				tServerEvent serverEvn = {};
				serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
				serverEvn.wParam = (DWORD_PTR)skillInfo;
				//serverEvn.lParam 
				CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
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
			if ((m_eRestraint & CAN_MOVE) == 0)
				return;

			CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
			IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
			Vec3 TargetPos = result.vCrossPoint;	// 클릭 좌표
			PathFinder()->FindPath(TargetPos);
		}


		if (KEY_TAP(KEY::Q))
		{
			// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[0]->GetCost() > m_fMP)
				return;

			if (m_Skill[0]->Use())
			{
				// 스킬 이벤트
			}
		}
		if (KEY_TAP(KEY::W))
		{
			// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[1]->GetCost() > m_fMP)
				return;

			if (m_Skill[1]->Use())
			{
				// 스킬 이벤트
			}
		}
		if (KEY_TAP(KEY::E))
		{
			// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[2]->GetCost() > m_fMP)
				return;

			if (m_Skill[2]->Use())
			{
				// 스킬 이벤트
			}
		}
		if (KEY_TAP(KEY::R))
		{
			// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[3]->GetCost() > m_fMP)
				return;

			if (m_Skill[3]->Use())
			{
				// 스킬 이벤트
			}
		}

		// 소환사 주문
	}
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
	if ((m_eRestraint & CAN_MOVE) == 0)
		return;

	// 이동
	if (PathFindMove(80, true))
	{
		// 이동 이벤트
		MoveEvent* evn = dynamic_cast<MoveEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_MOVE));
		if (evn != nullptr)
		{
			evn->Clear();
			evn->SetPlayerID(GetOwner()->GetID());
			evn->SetTargetPos(m_vNextPos);
			evn->SetFaceRot(m_fFaceRot);

			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
	else
	{
		// 이동 벡터값이 NaN -> 이동 불가, 멈춤
		StopEvent* evn = new StopEvent;
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
}

void CChampionScript::GetHit(CSkill* _skill)
{
	// 스킬 매니저 등에서 스킬 맵을 가지고 와 스킬을 까봄

	// 시전자 정보
	CGameObject* User = CUR_LEVEL->FindObjectByID(_skill->GetUserID());
	CUnitScript* UserScript = User->GetScript<CUnitScript>();

	float	UserAttackPower = UserScript->GetAttackPower();

	// 미완

}
