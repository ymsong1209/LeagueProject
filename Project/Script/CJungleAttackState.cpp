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
}

void CJungleAttackState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}

void CJungleAttackState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		//m_ptarget에 공격을함
		
	}
}


