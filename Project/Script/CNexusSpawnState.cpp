#include "pch.h"
#include "CNexusSpawnState.h"
#include "CNexusScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CNexusSpawnState::CNexusSpawnState()
{
}

CNexusSpawnState::~CNexusSpawnState()
{
}

void CNexusSpawnState::tick()
{
	// ���� �ִϸ��̼��� ������ Idle ���·� ��ȯ
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CNexusSpawnState::Enter()
{
	CUnitState::Enter();

	// HP ȸ��
	CNexusScript* NexusScript = GetOwner()->GetScript<CNexusScript>();
	NexusScript->SetCurHP(NexusScript->GetMaxHP());

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
	GetOwner()->Animator3D()->PlayOnce(L"nexus\\sruap_order_spawn.anm_skinned_mesh.001", true, 0.1f, 1.f);

	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(NexusScript->GetServerID(),
		L"nexus\\sruap_order_spawn.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.1f
		, 1.f);
}

void CNexusSpawnState::Exit()
{
	CUnitState::Exit();
}
