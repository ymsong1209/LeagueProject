#include "pch.h"
#include "CJinxDeathState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CUnitScript.h"

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
	wstring animName = L"Jinx\\Death_model.002";
	GetOwner()->Animator3D()->PlayOnce(animName, false);


	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, false, 0.0f);

	CChampionDeathState::Enter();
}  

void CJinxDeathState::Exit()
{
	//GetOwner()->Animator3D()->FindAnim(L"Jinx\\Death_model.002")->Reset();
	CChampionDeathState::Exit();
}

void CJinxDeathState::HandleEvent(CGameEvent& event)
{
	CChampionDeathState::HandleEvent(event);
}
