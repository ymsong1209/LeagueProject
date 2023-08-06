#include "pch.h"
#include "CGrompN2AState.h"
#include <Engine/CAnim3D.h>
#include "CGrompScript.h"
CGrompN2AState::CGrompN2AState()
{
}

CGrompN2AState::~CGrompN2AState()
{
}


void CGrompN2AState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1_n2a", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_idle1_n2a", false, false, true, 0.5f);
}

void CGrompN2AState::tick()
{
	CJungleN2AState::tick();
}

void CGrompN2AState::Exit()
{
	CJungleN2AState::Exit();
}

