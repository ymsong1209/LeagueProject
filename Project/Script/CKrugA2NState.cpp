#include "pch.h"
#include "CKrugA2NState.h"
#include <Engine/CAnim3D.h>

CKrugA2NState::CKrugA2NState()
{
}

CKrugA2NState::~CKrugA2NState()
{
}


void CKrugA2NState::Enter()
{
	GetOwner()->Animator3D()->PlayOnce(L"Krug\\krug_idle_ag2n", true, 0.5f);
}

void CKrugA2NState::tick()
{
	CJungleA2NState::tick();
}

void CKrugA2NState::Exit()
{
	CJungleA2NState::Exit();
}

