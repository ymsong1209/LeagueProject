#include "pch.h"
#include "CRedChaseState.h"
#include <Engine/CAnim3D.h>
#include "CRedScript.h"

CRedChaseState::CRedChaseState()
{
}

CRedChaseState::~CRedChaseState()
{
}


void CRedChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"jungle_red\\Run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"jungle_red\\Run", true, false, false, 0.f);
	CJungleChaseState::Enter();
}

void CRedChaseState::tick()
{
	CJungleChaseState::tick();
}

void CRedChaseState::Exit()
{
	CJungleChaseState::Exit();
}
