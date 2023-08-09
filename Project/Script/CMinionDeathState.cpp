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
		CSendServerEventMgr::GetInst()->SendDespawnPacket(GetOwner()->GetScript<CUnitScript>()->GetServerID(), 0.1f);
		//DestroyObject(GetOwner());
	}
}

void CMinionDeathState::Enter()
{
	CUnitState::Enter();

	CMinionScript* MinionScript = GetOwnerFSM()->GetOwner()->GetScript<CMinionScript>();
	UnitType Type = MinionScript->GetUnitType();

	wstring animName;
	
	switch (Type)
	{
	case UnitType::MELEE_MINION:
	{
		animName = L"minion_melee\\Death";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);
	}
	break;
	case UnitType::RANGED_MINION:
	{
		animName = L"minion_caster\\_chaos_death";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);
	}
	break;
	case UnitType::SIEGE_MINION:
	{
		animName = L"minion_siege\\cannon_chaos_attack1";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);
	}
	break;
	case UnitType::SUPER_MINION:
	{
		animName = L"minion_super\\Death_Base";
		GetOwner()->Animator3D()->PlayOnce(animName, true, 0.1f);
	}
	break;
	}

	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, animName, false, true, true, 0.1f);
}

void CMinionDeathState::Exit()
{
	CUnitState::Exit();
}
