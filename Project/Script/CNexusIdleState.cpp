#include "pch.h"
#include "CNexusIdleState.h"
#include "CNexusScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>


CNexusIdleState::CNexusIdleState()
{
}

CNexusIdleState::~CNexusIdleState()
{
}

void CNexusIdleState::tick()
{
	// ������ Broken���·� ��ȯ
	if (GetOwnerFSM()->GetOwner()->GetScript<CNexusScript>()->GetCurHP() <= 0)
	{
		GetOwnerFSM()->ChangeState(L"Broken");
		GetOwnerFSM()->GetOwner()->GetScript<CNexusScript>()->SetUnitDead(true);
	}
}

void CNexusIdleState::Enter()
{
	CUnitState::Enter();

	CNexusScript* NexusScript = GetOwner()->GetScript<CNexusScript>();

	//// ���� ����
	//if (NexusScript->GetFaction() == Faction::RED)
	//{
	//	// Red ���� ����
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);
	//
	//	// ���� ��Ŷ ����
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\inhibitor_red_Mtrl.mtrl");
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	//}
	//else
	//{
	//	// Red ���� ����
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Mtrl.mtrl"), 0);
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);
	//
	//	// ���� ��Ŷ ����
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\inhibitor_blue_Mtrl.mtrl");
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	//}

	// �ִϸ��̼� ���
	GetOwner()->Animator3D()->PlayRepeat(L"nexus\\sruap_order_idle.anm_skinned_mesh.001", true, true, 0.1f, 1.f);

	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(NexusScript->GetServerID(),
		L"nexus\\sruap_order_idle.anm_skinned_mesh.001"
		, true
		, true
		, true
		, 0.1f
		, 1.f);
}


void CNexusIdleState::Exit()
{
}

void CNexusIdleState::HandleEvent(CGameEvent& event)
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
