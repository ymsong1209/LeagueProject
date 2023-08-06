#include "pch.h"
#include "CMinionAttackState.h"
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "CMinionScript.h"
#include "CSkill.h"

CMinionAttackState::CMinionAttackState()
{
}

CMinionAttackState::~CMinionAttackState()
{
}

void CMinionAttackState::tick()
{
	CGameObject* Target = GetOwner()->GetScript<CMinionScript>()->GetTarget();

	// Ÿ���� �̹� �׾��ٸ� Walk��
	if (!GetOwner()->GetScript<CMinionScript>()->IsTargetValid(Target))
	{
		GetOwnerFSM()->ChangeState(L"Walk");
	}
	else
	{
		// Ÿ���� ��������� ���� ��Ÿ� ���� ���ٸ�
		if (!GetOwner()->GetScript<CMinionScript>()->IsTargetInRange(Target))
		{
			// Chase ���·� ��ȯ
			GetOwnerFSM()->ChangeState(L"Chase");
		}
		else
		{
			// Ÿ���� ����ְ�, ��Ÿ� ���� �ִٸ�

			// ���ݰ����� ��� ����
			if (GetOwner()->GetScript<CMinionScript>()->CanAttack())
			{
				CSkill* BasicAttack = GetOwner()->GetScript<CMinionScript>()->GetSkill(0);
				BasicAttack->SetUserObj(GetOwner());
				BasicAttack->SetTargetObj(Target);
				
				BasicAttack->Use();
			}

		}
	}
}

void CMinionAttackState::Enter()
{
	// �ִϸ��̼�
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

	// ���� ����ü
}

void CMinionAttackState::Exit()
{
}
