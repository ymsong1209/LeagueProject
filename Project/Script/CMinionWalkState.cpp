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
	// Waypoint�� ���� Move
	GetOwner()->GetScript<CMinionScript>()->Move();

	// ���� Ÿ���� �ִٸ�
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

	// ���� WayPoint�� ���� ��ã��
	Vec3 WayPoint = GetOwner()->GetScript<CMinionScript>()->GetWayPoint();
	GetOwner()->PathFinder()->FindPath(WayPoint);
}

void CMinionWalkState::Exit()
{
	// ��ã�� ������Ʈ�� ���� ��ΰ��� �ִٸ� Clear
	GetOwner()->PathFinder()->ClearPath();
}
