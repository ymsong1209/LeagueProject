#include "pch.h"
#include "CInhibitorRespawnState.h"
#include "CInhibitorScript.h"
#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>

CInhibitorRespawnState::CInhibitorRespawnState()
{
}

CInhibitorRespawnState::~CInhibitorRespawnState()
{
}

void CInhibitorRespawnState::tick()
{
	// ���� �ִϸ��̼��� ������ Idle ���·� ��ȯ
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CInhibitorRespawnState::Enter()
{
	CUnitState::Enter();

	// HP ȸ��
	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();
	InhibitorScript->SetCurHP(InhibitorScript->GetMaxHP());

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
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001", true, 0.1f, 0.2f);

	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_respawn.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.1f
		, 0.2f);
}

void CInhibitorRespawnState::Exit()
{
	CUnitState::Exit();
}
