#include "pch.h"
#include "CInhibitorIdleState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorIdleState::CInhibitorIdleState()
{
}

CInhibitorIdleState::~CInhibitorIdleState()
{
}

void CInhibitorIdleState::tick()
{
	// ������ Broken���·� ��ȯ
	if (GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()->GetCurHP() <= 0)
	{
		GetOwnerFSM()->ChangeState(L"Broken");
		GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()->SetUnitDead(true);
	}
}

void CInhibitorIdleState::Enter()
{
	CUnitState::Enter();

	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();

	// ���� ����
	if (InhibitorScript->GetFaction() == Faction::RED)
	{
		// Red ���� ����
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);

		// ���� ��Ŷ ����
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_red_Mtrl.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	}
	else
	{
		// Red ���� ����
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Mtrl.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);

		// ���� ��Ŷ ����
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_blue_Mtrl.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	}

	// �ִϸ��̼� ���
	GetOwner()->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f, 0.2f);

	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001"
		, true
		, true
		, true
		, 0.1f
		, 0.2f);
}

void CInhibitorIdleState::Exit()
{
	CUnitState::Exit();
}

void CInhibitorIdleState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	if (GetOwner()->GetScript<CUnitScript>()->IsUnitDead())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::GET_HIT:
	{
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// ���� Ÿ���� ������ ��쿡�� �̺�Ʈ�� ����
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			// �⺻ ���ݿ��� HP ����
						// �⺻ ���ݿ��� HP ����
			if (skilltype == SkillType::BASIC_ATTACK
				|| skilltype == SkillType::BASIC_RANGED_ATTACK
				|| skilltype == SkillType::JINX_BASIC_ATTACK
				|| skilltype == SkillType::MALPHITE_BASIC_ATTACK)
				GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}
	break;
	}
}

