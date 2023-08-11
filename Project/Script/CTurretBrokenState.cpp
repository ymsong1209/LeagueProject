#include "pch.h"
#include "CTurretBrokenState.h"
#include "CUnitScript.h"
#include <Engine\CAnim3D.h>

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

		// �ڽ�2(TurretBreak1) �ִϸ��̼� ������ ����
		if (!TurretBreak1Script->IsUnitDead() && TurretBreak1->Animator3D()->GetCurAnim()->IsFinish())
		{
			TurretBreak1Script->SetUnitDead(true);
			CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBreak1Script->GetServerID(), 0.f);
		}
	}

	if (TurretBreak2 != nullptr)
	{
		CUnitScript* TurretBreak2Script = TurretBreak2->GetScript<CUnitScript>();

		// �ڽ�3(TurretBreak2) �ִϸ��̼� ������ ����
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

	CUnitScript* TurretScript = GetOwner()->GetScript<CUnitScript>();

	// �þ� ���� ��� ����
	GetOwner()->Transform()->SetIsShootingRay(false);

	// ���� ���� �ѱ�
	if (TurretScript->GetFaction() == Faction::RED)
	{
		Ptr<CTexture> RedTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm.dds");

		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_red.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_red.mtrl"), 1);
		//GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, RedTex);
		//GetOwner()->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, RedTex);

		CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretScript->GetServerID(), 0, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm.dds");
		CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretScript->GetServerID(), 1, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm.dds");
	}
	else
	{
		Ptr<CTexture> BlueTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm_blue.dds");

		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_blue.mtrl"), 0);
		GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Break1_blue.mtrl"), 1);

		//GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, BlueTex);
		//GetOwner()->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, BlueTex);

		CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretScript->GetServerID(), 0, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm_blue.dds");
		CSendServerEventMgr::GetInst()->SendMtrlPacket(TurretScript->GetServerID(), 1, TEX_0, L"texture\\FBXTexture\\sruap_turret_chaos1_rubble_tx_cm_blue.dds");
	}

	// �ڽ�1(TurretBase) �����
	CGameObject* TurretBase = GetOwner()->FindChildObjByName(L"TurretBase");
	CUnitScript* TurretBaseScript = TurretBase->GetScript<CUnitScript>();
	if (TurretBase != nullptr && !TurretBaseScript->IsUnitDead())
	{
		TurretBaseScript->SetUnitDead(true);
		CSendServerEventMgr::GetInst()->SendDespawnPacket(TurretBaseScript->GetServerID(), 0.1f);
	}

	// �ڽ�2(TurretBreak1) �ִϸ��̼� ���
	CGameObject* TurretBreak1 = GetOwner()->FindChildObjByName(L"TurretBreak_1");
	CUnitScript* TurretBreak1Script = TurretBreak1->GetScript<CUnitScript>();
	if (TurretBreak1)
	{
		// TurretBreak1 ���� �ѱ�
		TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Cloth1_red.mtrl"), 0);
		TurretBreak1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break1_Mage_red.mtrl"), 1);
		TurretBreak1->Animator3D()->PlayOnce(L"turret_break1\\turret_break1", false);

		CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBreak1Script->GetServerID(),
			L"turret_break1\\turret_break1"
			, false
			, false
			, false
			, 0.f);
	}
	// �ڽ�3(TurretBreak2) �ִϸ��̼� ���
	CGameObject* TurretBreak2 = GetOwner()->FindChildObjByName(L"TurretBreak_2");
	CUnitScript* TurretBreak2Script = TurretBreak2->GetScript<CUnitScript>();
	if (TurretBreak2)
	{
		// TurretBreak2 ���� �ѱ�
		TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage1_red.mtrl"), 0);
		TurretBreak2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_break2_Mage2_red.mtrl"), 1);
		TurretBreak2->Animator3D()->PlayOnce(L"turret_break2\\turret_break2", false);

		CSendServerEventMgr::GetInst()->SendAnimPacket(TurretBreak2Script->GetServerID(),
			L"turret_break2\\turret_break2"
			, false
			, false
			, false
			, 0.f);
	}
}

void CTurretBrokenState::Exit()
{
	CUnitState::Exit();
}
