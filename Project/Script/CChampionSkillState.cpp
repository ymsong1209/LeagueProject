#include "pch.h"
#include "CChampionSkillState.h"
#include <Engine/CTimeMgr.h>
#include <Engine/CFsm.h>

CChampionSkillState::CChampionSkillState()
{
}

CChampionSkillState::~CChampionSkillState()
{
}

void CChampionSkillState::tick()
{
	// 캐스팅 시간
	if (m_fCurCastingTime > 0.f)
		m_fCurCastingTime -= DT;
	else
		m_fCurCastingTime = 0.f;

	// 애니메이션 끝났고, 캐스팅타임 끝났으면 Idle로 전환

}

void CChampionSkillState::Enter()
{
	CUnitState::Enter();
}

void CChampionSkillState::Exit()
{
	CUnitState::Exit();
	m_fCurCastingTime = m_fCastingTime;
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
