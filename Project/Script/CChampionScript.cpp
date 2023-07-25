#include "pch.h"
#include "CChampionScript.h"
#include  <algorithm>

#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>

#include "CGameEvent.h"
#include "CGameEventMgr.h"

#include "CSkill.h"

CChampionScript::CChampionScript(UINT ScriptType)
	: CUnitScript(ScriptType)
	, m_iLevel(1)
	, m_fExp(0)
	, m_EquippedSpell{}
	, m_fRespawnTime(5)
	, m_eCurCC(NONE)
	, m_eRestraint(DEFAULT)
	, m_Skill{}
{
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

	// FSM
	if (GetOwner()->Fsm() == nullptr)
		return;

	GetOwner()->Fsm()->ChangeState(L"Idle");

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
	m_fMana += 5.0f * DT;

	if (m_fHP > m_fMaxHP)
		m_fHP = m_fMaxHP;

	if (m_fMana > m_fMaxMana)
		m_fMana = m_fMaxMana;
}

void CChampionScript::GetInput()
{
	// 들어온 입력을 확인합니다.

	if (KEY_TAP(KEY::RBTN))
	{
		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = MainCam->GetRay();

		// 다른 오브젝트를 클릭했을 경우
		// ...
		/*
		if (오브젝트 클릭)
		{
		if (오브젝트 거리 - 플레이어 거리 < 공격 사거리)
		{
			일반공격;
			일반공격 이벤트 등록;
		}
		else
		{
			오브젝트 방향으로 오브젝트 중심에서 사거리만큼 떨어진 곳까지 걸어감
		}*/

		// 그 외(땅을 클릭한 경우)

		// 움직일 수 없는 상황인 경우 return
		if ((m_eRestraint & CAN_MOVE) == 0)
			return;

		CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(6)->GetParentObject()[0];
		IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
		Vec3 TargetPos = result.vCrossPoint;	// 클릭 좌표
		PathFinder()->FindPath(TargetPos);
	}


	if (KEY_TAP(KEY::Q))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[0]->GetCost() > m_fMana)
			return;

		if (m_Skill[0]->Use())
		{
			// 스킬 이벤트
		}
	}
	if (KEY_TAP(KEY::W))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[1]->GetCost() > m_fMana)
			return;

		if (m_Skill[1]->Use())
		{
			// 스킬 이벤트
		}
	}
	if (KEY_TAP(KEY::E))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[2]->GetCost() > m_fMana)
			return;

		if (m_Skill[2]->Use())
		{
			// 스킬 이벤트
		}
	}
	if (KEY_TAP(KEY::R))
	{
		// 스킬을 사용할 수 없는 상황 혹은 마나가 부족한 경우 return
		if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[3]->GetCost() > m_fMana)
			return;

		if (m_Skill[3]->Use())
		{
			// 스킬 이벤트
		}
	}

	// 소환사 주문

}

void CChampionScript::Move()
{
	// 움직일 수 없는 상황인 경우 return
	if ((m_eRestraint & CAN_MOVE) == 0)
		return;

	// 이동
	if (PathFindMove(200, true))
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
		StopEvent* evn = dynamic_cast<StopEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_STOP));
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
}
