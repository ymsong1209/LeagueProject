#include "pch.h"
#include "CGrompChaseState.h"
#include <Engine/CAnim3D.h>
#include "CGrompScript.h"
CGrompChaseState::CGrompChaseState()
{
}

CGrompChaseState::~CGrompChaseState()
{
}


void CGrompChaseState::Enter()
{
	GetOwner()->Animator3D()->PlayRepeat(L"gromp\\_run", false);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_run", true, false, false, 0.0f);
	CJungleChaseState::Enter();
}

void CGrompChaseState::tick()
{
	CJungleChaseState::tick();
}

void CGrompChaseState::Exit()
{
	CJungleChaseState::Exit();
}

