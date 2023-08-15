#include "pch.h"
#include "CInhibitorRespawnState.h"
#include "CInhibitorScript.h"
#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorRespawnState::CInhibitorRespawnState()
	: m_fRespawnTime(0)
	, m_fDeadTime(0)
	, m_bSounded(false)
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

	if (m_fDeadTime + 120 == CSendServerEventMgr::GetInst()->GetPlayTime() && !m_bSounded)
	{
		m_bSounded = true;

		// 억제기가 곧 재생성 됩니다. (30초뒤 억제기가 재생성 되어야 한다.)
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\announce_inhibitor_soon_respawn.mp3", 1, 0.5f, true, 0.f, Vec3(0, 0, 0), Faction::NONE); 
	}
}

void CInhibitorRespawnState::Enter()
{
	CUnitState::Enter();

	m_fDeadTime = CSendServerEventMgr::GetInst()->GetPlayTime();

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
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001", true, 0.2f, 0.21f);


	// 애니메이션 패킷 전송
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.2f
		, 0.21f); // 2분 30초는 0.21f  // 1.f 는 30초 // 0.5f는 60초
}

void CInhibitorRespawnState::Exit()
{
	CUnitState::Exit();
}
