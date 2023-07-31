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
	// ���� �ִϸ��̼��� ������ Idle ���·� ��ȯ
	if (GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CInhibitorSpawnState::Enter()
{
	// �������� �ʿ�(destroy..)
	CUnitState::Enter();
}

void CInhibitorSpawnState::Exit()
{
	CUnitState::Exit();
}
