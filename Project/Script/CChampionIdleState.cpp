#include "pch.h"
#include "CChampionIdleState.h"
#include <Engine/CFsm.h>

#include "CChampionAttackState.h"
#include "CChampionSkillState.h"
#include "CGameEvent.h"

#include "CUnitScript.h"

CChampionIdleState::CChampionIdleState()
{
}

CChampionIdleState::~CChampionIdleState()
{
}

void CChampionIdleState::tick()
{
}

void CChampionIdleState::Enter()
{

	CUnitState::Enter();
}

void CChampionIdleState::Exit()
{
	CUnitState::Exit();
}

void CChampionIdleState::HandleEvent(CGameEvent& event)
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
		if (AttackState != nullptr && AttackEvent->GetUserObj() == GetOwner())
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
		if(GetOwnerFSM()->FindState(L"Q") != nullptr)
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
		{
			PlayerEEvent* EEvent = dynamic_cast<PlayerEEvent*>(&event);

			CChampionSkillState* SkillState = dynamic_cast<CChampionSkillState*>(GetOwnerFSM()->FindState(L"E"));
			if (SkillState != nullptr)
			{
				SkillState->SetUserObj(EEvent->GetUserObj());
				SkillState->SetTargetObj(EEvent->GetTargetObj());
			}
			GetOwnerFSM()->ChangeState(L"E");
		}
		break;
	}
	case GAME_EVENT_TYPE::PLAYER_SKILL_R:
	{
		if (GetOwnerFSM()->FindState(L"R") != nullptr)
		{
			PlayerREvent* EEvent = dynamic_cast<PlayerREvent*>(&event);

			CChampionSkillState* SkillState = dynamic_cast<CChampionSkillState*>(GetOwnerFSM()->FindState(L"R"));
			if (SkillState != nullptr)
			{
				SkillState->SetUserObj(EEvent->GetUserObj());
				SkillState->SetTargetObj(EEvent->GetTargetObj());
			}
			GetOwnerFSM()->ChangeState(L"R");
		}
		break;
	}
	}
}
