#include "pch.h"
#include "CMinionDeathState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CMinionScript.h"

CMinionDeathState::CMinionDeathState()
{
}

CMinionDeathState::~CMinionDeathState()
{
}

void CMinionDeathState::tick()
{
}

void CMinionDeathState::Enter()
{
	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	MinionType Type = MinionScript->GetMinionType();
	switch (Type)
	{
	case MinionType::MELEE:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionmelee\\Walk");
	}
	break;
	case MinionType::RANGED:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionranged\\Walk");
	}
	break;
	case MinionType::SEIGE:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionsiege\\Walk");
	}
	break;
	case MinionType::SUPER:
	{
		//GetOwner()->Animator3D()->PlayLoop(L"minionsuper\\Walk");
	}
	break;
	}
}

void CMinionDeathState::Exit()
{
}
