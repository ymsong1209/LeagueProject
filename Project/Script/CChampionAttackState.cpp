#include "pch.h"
#include "CChampionAttackState.h"
#include <Engine/CFsm.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>

CChampionAttackState::CChampionAttackState()
{
}

CChampionAttackState::~CChampionAttackState()
{
}

void CChampionAttackState::tick()
{
	// 애니메이션이 끝날 경우, Idle로 전환
	if(GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");

}

void CChampionAttackState::Enter()
{
	CUnitState::Enter();
}

void CChampionAttackState::Exit()
{
	CUnitState::Exit();
}

void CChampionAttackState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;
	}
}
