#include "pch.h"
#include "CInhibitorIdleState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorIdleState::CInhibitorIdleState()
{
}

CInhibitorIdleState::~CInhibitorIdleState()
{
}

void CInhibitorIdleState::tick()
{
	// 죽으면 Broken상태로 전환
	if (GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()->IsUnitDead())
		GetOwnerFSM()->ChangeState(L"Broken");
}

void CInhibitorIdleState::Enter()
{
	// 재질설정 필요(destroy..)
	CUnitState::Enter();
}

void CInhibitorIdleState::Exit()
{
	CUnitState::Exit();
}
