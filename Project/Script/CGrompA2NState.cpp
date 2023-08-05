#include "pch.h"
#include "CGrompA2NState.h"
#include <Engine/CAnim3D.h>
#include "CGrompScript.h"

CGrompA2NState::CGrompA2NState()
{
}

CGrompA2NState::~CGrompA2NState()
{
}


void CGrompA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_idle1_a2n", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_idle1_a2n", false , true, 0.5f);
}

void CGrompA2NState::tick()
{
	CJungleA2NState::tick();
}

void CGrompA2NState::Exit()
{
	CJungleA2NState::Exit();
}

