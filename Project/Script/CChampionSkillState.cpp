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
	// ĳ���� �ð�
	if (m_fCurCastingTime > 0.f)
		m_fCurCastingTime -= DT;
	else
		m_fCurCastingTime = 0.f;

	// �ִϸ��̼� ������, ĳ����Ÿ�� �������� Idle�� ��ȯ

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
