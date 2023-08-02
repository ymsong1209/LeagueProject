#include "pch.h"
#include "CJungleAttackState.h"
#include <Engine/CAnim3D.h>

CJungleAttackState::CJungleAttackState()
{
}

CJungleAttackState::~CJungleAttackState()
{
}

void CJungleAttackState::Enter()
{
	//m_ptarget에 공격로직을 줌
}

void CJungleAttackState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Chase");
	}
}

void CJungleAttackState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}



