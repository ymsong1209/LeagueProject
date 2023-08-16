#include "pch.h"
#include "CTurretBrokenState.h"
#include "CUnitScript.h"
#include <Engine\CAnim3D.h>
#include <thread>

CTurretBrokenState::CTurretBrokenState()
{
}

CTurretBrokenState::~CTurretBrokenState()
{
}

void CTurretBrokenState::tick()
{
	CGameObject* TurretBreak1 = GetOwner()->FindChildObjByName(L"TurretBreak_1");
	CGameObject* TurretBreak2 = GetOwner()->FindChildObjByName(L"TurretBreak_2");
	
	if (TurretBreak1 != nullptr)
	{
		CUnitScript* TurretBreak1Script = TurretBreak1->GetScript<CUnitScript>();

		// 자식2(TurretBreak1) 애니메이션 끝나면 삭제
		if (!TurretBreak1Script->IsUnitDead() && TurretBreak1->Animator3D()->GetCurAnim()->IsFinish())
		{
			TurretBreak1Script->SetUnitDead(true);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBreak1Script->GetServerID(), 0.f);
		}
	}

	if (TurretBreak2 != nullptr)
	{
		CUnitScript* TurretBreak2Script = TurretBreak2->GetScript<CUnitScript>();

		// 자식3(TurretBreak2) 애니메이션 끝나면 삭제
		if (!TurretBreak2Script->IsUnitDead() && TurretBreak2->Animator3D()->GetCurAnim()->IsFinish())
		{
			TurretBreak2Script->SetUnitDead(true);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBreak2Script->GetServerID(), 0.f);
		}
	}
}

void CTurretBrokenState::Enter()
{
	CUnitState::Enter();
	
	CUnitScript* TurretScript = GetOwner()->GetScript<CUnitScript>();				// 포탑 잔해
	CGameObject* TurretBreak1 = GetOwner()->FindChildObjByName(L"TurretBreak_1");
	CUnitScript* TurretBreak1Script = TurretBreak1->GetScript<CUnitScript>();		// 포탑 Break1
	CGameObject* TurretBreak2 = GetOwner()->FindChildObjByName(L"TurretBreak_2");
	CUnitScript* TurretBreak2Script = TurretBreak2->GetScript<CUnitScript>();		// 포탑 Break2

	// 시야 제공 기능 삭제
	GetOwner()->Transform()->SetIsShootingRay(false);

	// 각 진영별 재질 켜기
	if (TurretScript->GetFaction() == Faction::RED)
	{
		// 잔해 Red 재질
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_red.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_red.mtrl"), 1);

		// Break1 Red 재질
		TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_red.mtrl"), 0);
		TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_red.mtrl"), 1);

		// Break2 Red 재질
		TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_red.mtrl"), 0);
		TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_red.mtrl"), 1);

		// 서버에 잔해 Red 재질 변경 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretScript->GetServerID(), 0, L"material\\turret_rubble_Rubble_red.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretScript->GetServerID(), 1, L"material\\turret_rubble_Break1_red.mtrl");

		// 서버에 Break1 Red 재질 변경 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak1Script->GetServerID(), 0, L"material\\turret_break1_Cloth1_red.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak1Script->GetServerID(), 1, L"material\\turret_break1_Mage_red.mtrl");

		// 서버에 Break2 Red 재질 변경 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak2Script->GetServerID(), 0, L"material\\turret_break2_Mage1_red.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak2Script->GetServerID(), 1, L"material\\turret_break2_Mage2_red.mtrl");
	}
	else
	{
		// 잔해 Blue 재질
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_blue.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_blue.mtrl"), 1);

		// Break1 Blue 재질
		TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_blue.mtrl"), 0);
		TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_blue.mtrl"), 1);

		// Break2 Blue 재질
		TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_blue.mtrl"), 0);
		TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_blue.mtrl"), 1);

		// 서버에 잔해 Blue 재질 변경 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretScript->GetServerID(), 0, L"material\\turret_rubble_Rubble_blue.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretScript->GetServerID(), 1, L"material\\turret_rubble_Break1_blue.mtrl");

		// 서버에 Break1 Blue 재질 변경 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak1Script->GetServerID(), 0, L"material\\turret_break1_Cloth1_blue.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak1Script->GetServerID(), 1, L"material\\turret_break1_Mage_blue.mtrl");

		// 서버에 Break2 Blue 재질 변경 패킷 전송
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak2Script->GetServerID(), 0, L"material\\turret_break2_Mage1_blue.mtrl");
		CSendServerEventMgr::GetInst()->SendSetMtrlPacket(TurretBreak2Script->GetServerID(), 1, L"material\\turret_break2_Mage2_blue.mtrl");
	}

	// 자식1(TurretBase) 지우기
	CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
	CUnitScript* TurretBaseScript = TurretBase->GetScript<CUnitScript>();
	if (TurretBase != nullptr && !TurretBaseScript->IsUnitDead())
	{
		TurretBaseScript->SetUnitDead(true);
		CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBaseScript->GetServerID(), 0.1f);
	}

	// Break1,2 애니메이션 재생 & 애니메이션 패킷 전송
	TurretBreak1->Animator3D()->PlayOnce(L"turret_break1\\turret_break1", false);
	TurretBreak2->Animator3D()->PlayOnce(L"turret_break2\\turret_break2", false);

	CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBreak1Script->GetServerID(),
		L"turret_break1\\turret_break1"
		, false
		, false
		, false
		, 0.f
		, 1.f);

	CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBreak2Script->GetServerID(),
		L"turret_break2\\turret_break2"
		, false
		, false
		, false
		, 0.f
		, 1.f);

	CSendServerEventMgr::GetInst()->SendSoundPacket(L"sound3d\\sfx_TurretBroken.mp3", 1, 0.4f, true, 200.f, GetOwner()->Transform()->GetRelativePos(), Faction::NONE);
}

void CTurretBrokenState::Exit()
{
	CUnitState::Exit();
}
