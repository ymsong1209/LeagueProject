#include "pch.h"
#include "CChampionSkillState.h"
#include <Engine/CFsm.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>

CChampionSkillState::CChampionSkillState()
{
}

CChampionSkillState::~CChampionSkillState()
{
}

void CChampionSkillState::tick()
{
	// 애니메이션이 끝날 경우, Idle로 전환
	if (GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");

}

void CChampionSkillState::Enter()
{
	CUnitState::Enter();
}

void CChampionSkillState::Exit()
{
	CUnitState::Exit();
}

void CChampionSkillState::HandleEvent(CGameEvent& event)
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
