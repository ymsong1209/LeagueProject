#include "pch.h"
#include "CTurretHPUIScript.h"
#include "CUnitScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>

void CTurretHPUIScript::begin()
{
	//GetOwner()->ChangeLayer(31);
	Transform()->SetNoParentaffected(true);
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

	ParentUnitType = GetOwner()->GetParent()->GetScript<CUnitScript>()->GetUnitType();
	Faction FactionType = GetOwner()->GetParent()->GetScript<CUnitScript>()->GetFaction();
	Vec3 Pos = GetOwner()->GetParent()->Transform()->GetRelativePos();

	switch (ParentUnitType)
	{
	case UnitType::TURRET:
	{
		if (FactionType == Faction::BLUE)
			MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueTurretHPBar.mtrl"), 0);
		else if (FactionType == Faction::RED)
			MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedTurretHPBar.mtrl"), 0);

		Transform()->SetRelativeScale(Vec3(71.6f, 10.5f, 1.f));
		m_vOffsetBarPos = Vec3(Pos.x, Pos.y + 80.f, Pos.z + 20.f);

		break;
	}
	case UnitType::INHIBITOR:
	{
		if (FactionType == Faction::BLUE)
			MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueInhibitorHPBar.mtrl"), 0);
		else if (FactionType == Faction::RED)
			MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedInhibitorHPBar.mtrl"), 0);

		Transform()->SetRelativeScale(Vec3(45.3f, 6.9f, 1.f));
		m_vOffsetBarPos = Vec3(Pos.x, Pos.y + 40.f, Pos.z + 27.f);

		break;
	}
	case UnitType::NEXUS:
	{
		if (FactionType == Faction::BLUE)
			MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueTurretHPBar.mtrl"), 0);
		else if (FactionType == Faction::RED)
			MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedTurretHPBar.mtrl"), 0);

		Transform()->SetRelativeScale(Vec3(83.4f, 10.9f, 1.f));
		m_vOffsetBarPos = Vec3(Pos.x, Pos.y + 50.f, Pos.z + 40.f);

		break;
	}
	}

}

void CTurretHPUIScript::tick()
{
	if (GetOwner()->GetParent())
	{
		CGameObject* ParentObj = GetOwner()->GetParent();
		bool IsCull = ParentObj->GetRenderComponent()->IsCulled();
		bool UseRaySight = ParentObj->GetRenderComponent()->IsUsingRaySightCulling();
		if (IsCull && UseRaySight)
		{
			GetOwner()->GetRenderComponent()->SetSortExcept(true);
			return;
		}

		CUnitScript* UnitScript = ParentObj->GetScript<CUnitScript>();
		UnitType UnitType = UnitScript->GetUnitType();

		m_fCurHP = ParentObj->GetScript<CUnitScript>()->GetCurHP();
		m_fTotalHP = ParentObj->GetScript<CUnitScript>()->GetMaxHP();

		if (m_fCurHP <= 0.f)
		{
			if (ParentUnitType == UnitType::INHIBITOR)
				MeshRender()->SetSortExcept(true);
			else
				DestroyObject(GetOwner());
		}

		Transform()->SetAbsolute(false);
		Vec3 Pos = ParentObj->Transform()->GetRelativePos();
		Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
		Transform()->SetRelativePos(m_vOffsetBarPos);

		//----------HP------------
		////======µð¹ö±ë¿ë=========
		//m_fCurHP = 570.f;
		//m_fTotalHP = 1000.f;
		////====================
		m_fRatio = m_fCurHP / m_fTotalHP;
		if (m_fCurHP >= m_fTotalHP)
			m_fCurHP = m_fTotalHP;

		if (m_fCurHP <= 0.f)
			m_fCurHP = 0.f;
		//-------------------------
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fRatio);
	}
}

void CTurretHPUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CTurretHPUIScript::CTurretHPUIScript()
	:CScript((UINT)SCRIPT_TYPE::TURRETHPUISCRIPT)
{
}

CTurretHPUIScript::~CTurretHPUIScript()
{
}
