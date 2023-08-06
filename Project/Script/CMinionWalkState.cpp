#include "pch.h"
#include "CMinionWalkState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CMinionScript.h"

CMinionWalkState::CMinionWalkState()
{
}

CMinionWalkState::~CMinionWalkState()
{
}

void CMinionWalkState::tick()
{
	// Waypoint를 향해 Move
	GetOwner()->GetScript<CMinionScript>()->Move();

	// 공격 타겟이 있다면
	if (GetOwner()->GetScript<CMinionScript>()->GetTarget())
		GetOwnerFSM()->ChangeState(L"Attack");
}

void CMinionWalkState::Enter()
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

	// 다음 WayPoint를 향해 길찾기
	Vec3 WayPoint = GetOwner()->GetScript<CMinionScript>()->GetWayPoint();
	GetOwner()->PathFinder()->FindPath(WayPoint);
}

void CMinionWalkState::Exit()
{
	// 길찾기 컴포넌트에 남은 경로값이 있다면 Clear
	GetOwner()->PathFinder()->ClearPath();
}
