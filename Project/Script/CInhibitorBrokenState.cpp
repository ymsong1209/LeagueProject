#include "pch.h"
#include "CInhibitorBrokenState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>
#include <Engine/CTimeMgr.h>

CInhibitorBrokenState::CInhibitorBrokenState()
	: m_fRespawnTime(0)
{
}

CInhibitorBrokenState::~CInhibitorBrokenState()
{
}

void CInhibitorBrokenState::tick()
{
	// RespawnTime이 지나면 Respawn 상태로 전환
	m_fRespawnTime -= DT;

	if(m_fRespawnTime <=0)
		GetOwnerFSM()->ChangeState(L"Respawn");
}

void CInhibitorBrokenState::Enter()
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
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_death.anm_skinned_mesh.001", false);

	// 애니메이션 패킷 전송
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_death.anm_skinned_mesh.001"
		, false
		, false
		, false
		, 0.f
		, 1.f);
}

void CInhibitorBrokenState::Exit()
{
	CUnitState::Exit();
}
