#include "pch.h"
#include "CJinxDeathState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>


CJinxDeathState::CJinxDeathState()
{
	SetName(L"Death");
}

CJinxDeathState::~CJinxDeathState()
{
}

void CJinxDeathState::tick()
{
}

void CJinxDeathState::Enter()
{
	GetOwner()->Animator3D()->SetRepeat(false);
	//GetOwner()->Animator3D()->Play(L"Jinx\\Death_model.002", false, 0.1f);
	GetOwner()->Animator3D()->PlayOnce(L"Jinx\\Death_model.002");
	
	CChampionDeathState::Enter();
}  

void CJinxDeathState::Exit()
{
	GetOwner()->Animator3D()->FindAnim(L"Jinx\\Death_model.002")->Reset();
	CChampionDeathState::Exit();
}

void CJinxDeathState::HandleEvent(CGameEvent& event)
{
	CChampionDeathState::HandleEvent(event);
}
