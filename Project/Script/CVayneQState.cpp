#include "pch.h"
#include "CVayneQState.h"

CVayneQState::CVayneQState()
{
}

CVayneQState::~CVayneQState()
{
}

void CVayneQState::tick()
{
}

void CVayneQState::Enter()
{
	CChampionSkillState::Enter();
}

void CVayneQState::Exit()
{
	CChampionSkillState::Exit();
}

void CVayneQState::HandleEvent(CGameEvent& event)
{
	CChampionSkillState::HandleEvent(event);
}

 
