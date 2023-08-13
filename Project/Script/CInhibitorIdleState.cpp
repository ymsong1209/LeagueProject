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
	// ������ Broken���·� ��ȯ
	if (GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()->IsUnitDead())
		GetOwnerFSM()->ChangeState(L"Broken");
}

void CInhibitorIdleState::Enter()
{
	// �������� �ʿ�(destroy..)
	CUnitState::Enter();
}

void CInhibitorIdleState::Exit()
{
	CUnitState::Exit();
}
