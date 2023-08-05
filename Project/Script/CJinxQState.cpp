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
	//wstring animName = L"Jinx\\Death_model.002";
	//GetOwner()->Animator3D()->PlayOnce(animName, false, 0.1f);
	//
	// 
	//SendAnimPacket(animName, false, true, 0.1f);

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
