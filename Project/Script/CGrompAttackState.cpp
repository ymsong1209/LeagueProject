#include "pch.h"
#include "CGrompAttackState.h"
#include "CGrompScript.h"
CGrompAttackState::CGrompAttackState()
{
}

CGrompAttackState::~CGrompAttackState()
{
}

void CGrompAttackState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"gromp\\_attack1",true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"gromp\\_attack1", false, true, 0.5f);
	CJungleAttackState::Enter();
}

void CGrompAttackState::tick()
{
	CJungleAttackState::tick();
}


void CGrompAttackState::Exit()
{
	CJungleAttackState::Exit();
}



