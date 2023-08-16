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
	// 스폰 애니메이션이 끝나면 Idle 상태로 전환
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CNexusSpawnState::Enter()
{
	CUnitState::Enter();

	// HP 회복
	CNexusScript* NexusScript = GetOwner()->GetScript<CNexusScript>();
	NexusScript->SetCurHP(NexusScript->GetMaxHP());

	//// 재질 설정
	//if (NexusScript->GetFaction() == Faction::RED)
	//{
	//	// Red 재질 설정
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);
	//
	//	// 재질 패킷 전송
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\inhibitor_red_Mtrl.mtrl");
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	//}
	//else
	//{
	//	// Red 재질 설정
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Mtrl.mtrl"), 0);
	//	GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);
	//
	//	// 재질 패킷 전송
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\inhibitor_blue_Mtrl.mtrl");
	//	CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	//}

	// 애니메이션 재생
	GetOwner()->Animator3D()->PlayOnce(L"nexus\\sruap_order_spawn.anm_skinned_mesh.001", true, 0.1f, 1.f);

	// 애니메이션 패킷 전송
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
