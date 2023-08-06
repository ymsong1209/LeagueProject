#include "pch.h"
#include "CChampionRecallState.h"
#include <Engine/CFsm.h>
#include <Engine\CTimeMgr.h>

#include "CChampionAttackState.h"
#include "CGameEvent.h"
#include "CUnitScript.h"
#include "CChampionSkillState.h"

CChampionRecallState::CChampionRecallState()
	: m_fRecallTime(8.0f)
{
}

CChampionRecallState::~CChampionRecallState()
{
}

void CChampionRecallState::tick()
{
	m_fRecallTime -= DT;
	if (m_fRecallTime <= 0)
	{
		// 우물로 Pos 변경
		GetOwnerFSM()->ChangeState(L"Respawn");
	}
}

void CChampionRecallState::Enter()
{
	CUnitState::Enter();
}

void CChampionRecallState::Exit()
{
	// 귀환 시간 초기화
	m_fRecallTime = 8.f;

	CUnitState::Exit();
}

void CChampionRecallState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;

	case GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK:
	{
		BasicAttackEvent* AttackEvent = dynamic_cast<BasicAttackEvent*>(&event);

		CChampionAttackState* AttackState = dynamic_cast<CChampionAttackState*>(GetOwnerFSM()->FindState(L"Attack"));
		if (AttackState != nullptr)
		{
			AttackState->SetUserObj(AttackEvent->GetUserObj());
			AttackState->SetTargetObj(AttackEvent->GetTargetObj());
		}
		GetOwnerFSM()->ChangeState(L"Attack");
	}
	break;
	case GAME_EVENT_TYPE::GET_HIT:
	{
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// 맞은 타겟이 본인인 경우에만 이벤트에 반응
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;

	case GAME_EVENT_TYPE::PLAYER_MOVE:
		GetOwnerFSM()->ChangeState(L"Walk");
		break;
	case GAME_EVENT_TYPE::PLAYER_SKILL_Q:
	{
		if (GetOwnerFSM()->FindState(L"Q") != nullptr)
			GetOwnerFSM()->ChangeState(L"Q");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_W:
	{
		if (GetOwnerFSM()->FindState(L"W") != nullptr)
		{
			PlayerWEvent* WEvent = dynamic_cast<PlayerWEvent*>(&event);

			CChampionSkillState* SkillState = dynamic_cast<CChampionSkillState*>(GetOwnerFSM()->FindState(L"W"));
			if (SkillState != nullptr)
			{
				SkillState->SetUserObj(WEvent->GetUserObj());
				SkillState->SetTargetObj(WEvent->GetTargetObj());
			}
			GetOwnerFSM()->ChangeState(L"W");
		}
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_E:
	{
		if (GetOwnerFSM()->FindState(L"E") != nullptr)
			GetOwnerFSM()->ChangeState(L"E");
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_R:
	{
		if (GetOwnerFSM()->FindState(L"R") != nullptr)
			GetOwnerFSM()->ChangeState(L"R");
		break;
	}
	}
}
