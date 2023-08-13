#include "pch.h"
#include "CInhibitorRespawnState.h"
#include "CInhibitorScript.h"
#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorRespawnState::CInhibitorRespawnState()
	: m_fRespawnTime(0)
{
}

CInhibitorRespawnState::~CInhibitorRespawnState()
{
}

void CInhibitorRespawnState::tick()
{
	m_fRespawnTime -= DT;

	// 스폰 애니메이션이 끝나면 && m_fRespawnTime이 끝나면 Idle 상태로 전환
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish() && m_fRespawnTime <=0)
	{
		// HP 회복
		CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();
		InhibitorScript->SetCurHP(InhibitorScript->GetMaxHP());
		InhibitorScript->SetUnitDead(false);

		GetOwnerFSM()->ChangeState(L"Idle");
	}
}

void CInhibitorRespawnState::Enter()
{
	CUnitState::Enter();
	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();

	// RespawnTime  지정
	m_fRespawnTime = InhibitorScript->GetRespawnTime();

	// 재질 설정
	if (InhibitorScript->GetFaction() == Faction::RED)
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_alpha.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Destroyed_Mtrl.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_alpha.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_red_Destroyed_Mtrl.mtrl");
	}
	else
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_alpha.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Destroyed_Mtrl.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_alpha.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_blue_Destroyed_Mtrl.mtrl");
	}

	// 애니메이션 재생
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001", true, 0.2f, 0.5f);

	// 애니메이션 패킷 전송
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.2f
		, 0.5f);
}

void CInhibitorRespawnState::Exit()
{
	CUnitState::Exit();
}
