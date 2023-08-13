#include "pch.h"
#include "CInhibitorSpawnState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorSpawnState::CInhibitorSpawnState()
{
}

CInhibitorSpawnState::~CInhibitorSpawnState()
{
}

void CInhibitorSpawnState::tick()
{
	// 스폰 애니메이션이 끝나면 Idle 상태로 전환
	if (GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CInhibitorSpawnState::Enter()
{
	// 재질설정 필요(destroy..)
	CUnitState::Enter();
}

void CInhibitorSpawnState::Exit()
{
	CUnitState::Exit();
}
