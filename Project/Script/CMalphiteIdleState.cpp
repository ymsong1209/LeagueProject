#include "pch.h"
#include "CMalphiteIdleState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>

#include "CSendServerEventMgr.h"
#include "CUnitScript.h"
#include "CChampionAttackState.h"
#include "CChampionSkillState.h"
#include <Engine/CRenderMgr.h>
#include "CMalphiteRState.h"

CMalphiteIdleState::CMalphiteIdleState()
	:m_iIdleAnimNum(1)
{
	SetName(L"Idle");
}

CMalphiteIdleState::~CMalphiteIdleState()
{
}


void CMalphiteIdleState::Enter()
{
	m_iIdleAnimNum = 1;

	wstring baseName = L"Malphite\\Idle";
	baseName += std::to_wstring(m_iIdleAnimNum);
	GetOwner()->Animator3D()->PlayOnce(baseName, true, 0.5f);

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, baseName, false, false, true, 0.5f);

	CChampionIdleState::Enter();
}

void CMalphiteIdleState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		++m_iIdleAnimNum;
		if (m_iIdleAnimNum > 4) m_iIdleAnimNum = 1;
		wstring baseName = L"Malphite\\Idle";
		baseName += std::to_wstring(m_iIdleAnimNum);
		GetOwner()->Animator3D()->GetCurAnim()->Reset();
		GetOwner()->Animator3D()->PlayOnce(baseName);
	
		UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
		CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, baseName, false, false, false, 0.f);
	}
}


void CMalphiteIdleState::Exit()
{
	m_iIdleAnimNum = 1;

	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	CChampionIdleState::Exit();
}

void CMalphiteIdleState::HandleEvent(CGameEvent& event)
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
		if (GetOwnerFSM()->FindState(L"Q") != nullptr)
		{
			PlayerQEvent* QEvent = dynamic_cast<PlayerQEvent*>(&event);

			CChampionSkillState* SkillState = dynamic_cast<CChampionSkillState*>(GetOwnerFSM()->FindState(L"Q"));
			if (SkillState != nullptr)
			{
				SkillState->SetUserObj(QEvent->GetUserObj());
				SkillState->SetTargetObj(QEvent->GetTargetObj());
			}
			GetOwnerFSM()->ChangeState(L"Q");
		}
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

			CMalphiteRState* SkillState = dynamic_cast<CMalphiteRState*>(GetOwnerFSM()->FindState(L"R"));
			if (SkillState != nullptr)
			{
				SkillState->SetUserObj(EEvent->GetUserObj());
				SkillState->SetTargetObj(nullptr);

				CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
				tRay ray = MainCam->GetRay();

				CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"LoLMapCollider");
				IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
				Vec3 TargetPos = result.vCrossPoint;	// 클릭 좌표

				SkillState->SetTargetPos(TargetPos);
			}
			GetOwnerFSM()->ChangeState(L"R");
		}
		break;
	}
	}
}
