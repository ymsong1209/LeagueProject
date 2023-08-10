#include "pch.h"
#include "CJinxRespawnState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CUnitScript.h"
#include "CJinxScript.h"

CJinxRespawnState::CJinxRespawnState()
{
	SetName(L"Respawn");
}

CJinxRespawnState::~CJinxRespawnState()
{
}

void CJinxRespawnState::tick()
{
	CChampionRespawnState::tick();
}

void CJinxRespawnState::Enter()
{
	wstring animName = L"Jinx\\Respawn";
	GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);


	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);
	
	CChampionRespawnState::Enter();
}

void CJinxRespawnState::Exit()
{
	CChampionRespawnState::Exit();
}

void CJinxRespawnState::HandleEvent(CGameEvent& event)
{
	CChampionRespawnState::HandleEvent(event);
}
