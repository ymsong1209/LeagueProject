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
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
	{
		DestroyObject(GetOwner());
	}
}

void CMinionDeathState::Enter()
{
	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	UnitType Type = MinionScript->GetUnitType();

	switch (Type)
	{
	case UnitType::MELEE_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_melee\\Death", true, 0.1f);
	}
	break;
	case UnitType::RANGED_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_caster\\_chaos_death", true, 0.1f);
	}
	break;
	case UnitType::SIEGE_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_siege\\cannon_chaos_attack1", true, 0.1f);
	}
	break;
	case UnitType::SUPER_MINION:
	{
		GetOwner()->Animator3D()->PlayOnce(L"minion_super\\Death_Base", true, 0.1f);
	}
	break;
	}
}

void CMinionDeathState::Exit()
{
}
