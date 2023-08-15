#include "pch.h"
#include "CNexusBrokenState.h"
#include "CNexusScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>


CNexusBrokenState::CNexusBrokenState()
{
}

CNexusBrokenState::~CNexusBrokenState()
{
}

void CNexusBrokenState::tick()
{
}

void CNexusBrokenState::Enter()
{
	CUnitState::Enter();

	CNexusScript* NexusScript = GetOwner()->GetScript<CNexusScript>();

	// ���� ����
	if (NexusScript->GetFaction() == Faction::RED)
	{
		// Red ���� ����
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_Red.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_alpha.mtrl"), 1);

		// ���� ��Ŷ ����
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\nexus_Destroyed_Red.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\nexus_Destroyed_alpha.mtrl");
	}
	else
	{
		// Red ���� ����
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_Blue.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_alpha.mtrl"), 1);

		// ���� ��Ŷ ����
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\nexus_Destroyed_Blue.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\nexus_Destroyed_alpha.mtrl");
	}

	// �ִϸ��̼� ���
	GetOwner()->Animator3D()->PlayOnce(L"nexus\\sruap_order_death.anm_skinned_mesh.001", true, 0.1f, 0.8f);

	// �ִϸ��̼� ��Ŷ ����
	CSendServerEventMgr::GetInst()->SendAnimPacket(NexusScript->GetServerID(),
		L"nexus\\sruap_order_death.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.1f
		, 0.8f);
}

void CNexusBrokenState::Exit()
{
	CUnitState::Exit();
}

