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

	// ���� �ִϸ��̼��� ������ && m_fRespawnTime�� ������ Idle ���·� ��ȯ
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish() && m_fRespawnTime <=0)
	{
		// HP ȸ��
		CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();
		InhibitorScript->SetCurHP(InhibitorScript->GetMaxHP());
		InhibitorScript->SetUnitDead(false);

		GetOwnerFSM()->ChangeState(L"Idle");
	}

	if (m_fDeadTime + 120 == CSendServerEventMgr::GetInst()->GetPlayTime() && !m_bSounded)
	{
		m_bSounded = true;

		// �����Ⱑ �� ����� �˴ϴ�. (30�ʵ� �����Ⱑ ����� �Ǿ�� �Ѵ�.)
		CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\announce_inhibitor_soon_respawn.mp3", 1, 0.5f, true, 0.f, Vec3(0, 0, 0), Faction::NONE); 
	}
}

void CInhibitorRespawnState::Enter()
{
	CUnitState::Enter();

	m_fDeadTime = CSendServerEventMgr::GetInst()->GetPlayTime();

	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();

	// RespawnTime  ����
	m_fRespawnTime = InhibitorScript->GetRespawnTime();

	// ���� ����
	if (InhibitorScript->GetFaction() == Faction::RED)
	{
		// Red ���� ����
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_alpha.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Destroyed_Mtrl.mtrl"), 1);

		// ���� ��Ŷ ����
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_alpha.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_red_Destroyed_Mtrl.mtrl");
	}
	else
	{
		// Red ���� ����
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_alpha.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_blue_Destroyed_Mtrl.mtrl"), 1);

		// ���� ��Ŷ ����
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 0, L"material\\inhibitor_alpha.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(InhibitorScript->GetServerID(), 1, L"material\\inhibitor_blue_Destroyed_Mtrl.mtrl");
	}

	// �ִϸ��̼� ���
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001", true, 0.2f, 0.21f);


	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.2f
		, 0.21f); // 2�� 30�ʴ� 0.21f  // 1.f �� 30�� // 0.5f�� 60��
}

void CInhibitorRespawnState::Exit()
{
	CUnitState::Exit();
}
