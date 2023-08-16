#include "pch.h"
#include "CNexusBrokenState.h"
#include "CNexusScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>
#include <thread>

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

	// 재질 설정
	if (NexusScript->GetFaction() == Faction::RED)
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_Red.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_alpha.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\nexus_Destroyed_Red.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\nexus_Destroyed_alpha.mtrl");
	}
	else
	{
		// Red 재질 설정
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_Blue.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Destroyed_alpha.mtrl"), 1);

		// 재질 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 0, L"material\\nexus_Destroyed_Blue.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(NexusScript->GetServerID(), 1, L"material\\nexus_Destroyed_alpha.mtrl");
	}

	// 애니메이션 재생
	GetOwner()->Animator3D()->PlayOnce(L"nexus\\sruap_order_death.anm_skinned_mesh.001", true, 0.1f, 0.8f);

	// 애니메이션 패킷 전송
	CSendServerEventMgr::GetInst()->SendAnimPacket(NexusScript->GetServerID(),
		L"nexus\\sruap_order_death.anm_skinned_mesh.001"
		, false
		, false
		, true
		, 0.1f
		, 0.8f);


	// 4초뒤 터지는 소리
	thread t1([=]() {
		Sleep(4000);
	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound2d\\sfx_nexus_boom.mp3", 1, 0.5f, true, 0.f, Vec3(0, 0, 0), Faction::NONE);
		});
	t1.detach();
}

void CNexusBrokenState::Exit()
{
	CUnitState::Exit();
}

