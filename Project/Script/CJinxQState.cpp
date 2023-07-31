#include "pch.h"
#include "CJinxQState.h"

CJinxQState::CJinxQState()
{
}

CJinxQState::~CJinxQState()
{
}

void CJinxQState::tick()
{
}

void CJinxQState::Enter()
{
	CChampionSkillState::Enter();
}

void CJinxQState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxQState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}
