#include "pch.h"
#include "CInhibitorSpawnState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorSpawnState::CInhibitorSpawnState()
{
}

CInhibitorSpawnState::~CInhibitorSpawnState()
{
}

void CInhibitorSpawnState::tick()
{
	// 스폰 애니메이션이 끝나면 Idle 상태로 전환
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CInhibitorSpawnState::Enter()
{
	CUnitState::Enter();

	// HP 회복
	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();
	InhibitorScript->SetCurHP(InhibitorScript->GetMaxHP());

	// 재질 설정
	if (InhibitorScript->GetFaction() == Faction::RED)
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_red_Mtrl.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	}
	else
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Mtrl.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_Destroyed_alpha.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_blue_Mtrl.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_Destroyed_alpha.mtrl");
	}

	// 애니메이션 재생
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_spawn.anm_skinned_mesh.001", true, 0.1f, 0.2f);

	// 애니메이션 패킷 전송
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_spawn.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.1f
		, 0.2f);
}

void CInhibitorSpawnState::Exit()
{
	CUnitState::Exit();
}
