#include "pch.h"
#include "CInhibitorBrokenState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>
#include <Engine/CTimeMgr.h>

CInhibitorBrokenState::CInhibitorBrokenState()
{
}

CInhibitorBrokenState::~CInhibitorBrokenState()
{
}

void CInhibitorBrokenState::tick()
{
	// Broken �ִϸ��̼��� ������ Respawn ���·� ��ȯ
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
	{
		GetOwnerFSM()->ChangeState(L"Respawn");
	}
}

void CInhibitorBrokenState::Enter()
{
	CUnitState::Enter();
	CInhibitorScript* InhibitorScript = GetOwner()->GetScript<CInhibitorScript>();

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
	GetOwner()->Animator3D()->PlayOnce(L"Inhibitor\\inhibitor_death.anm_skinned_mesh.001", true, 0.1f, 1.f);

	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(InhibitorScript->GetServerID(),
		L"Inhibitor\\inhibitor_death.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.1f
		, 1.f);
}

void CInhibitorBrokenState::Exit()
{
	CUnitState::Exit();
}
