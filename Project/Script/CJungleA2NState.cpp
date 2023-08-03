#include "pch.h"
#include "CJungleA2NState.h"
#include <Engine/CAnim3D.h>
CJungleA2NState::CJungleA2NState()
{
}

CJungleA2NState::~CJungleA2NState()
{
}


void CJungleA2NState::Enter()
{
}

void CJungleA2NState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Idle");
	}
}

void CJungleA2NState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}

