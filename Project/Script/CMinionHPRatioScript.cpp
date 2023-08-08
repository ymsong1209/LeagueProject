#include "pch.h"
#include "CMinionHPRatioScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>
#include "CUnitScript.h"

void CMinionHPRatioScript::begin()
{
	Transform()->SetRelativeScale(Vec3(154.f, 15.5f, 1.f));
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//===================인스턴싱 모드XXX==========================
	CGameObject* ParentObj = GetOwner()->GetParent();
	Faction OwnerFaction = ParentObj->GetScript<CUnitScript>()->GetFaction();
	if (OwnerFaction == Faction::BLUE)
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionBar.mtrl"), 0);
	else if (OwnerFaction == Faction::RED)
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedMinionBar.mtrl"), 0);

	//===================인스턴싱 모드==========================
	// 
	//CGameObject* ParentObj = GetOwner()->GetParent();
	//Faction OwnerFaction = ParentObj->GetScript<CUnitScript>()->GetFaction();
	//if (OwnerFaction == Faction::BLUE)
		//MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionBarHP.mtrl"), 0);
	//else if (OwnerFaction == Faction::RED)
		//MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedMinionBarHP.mtrl"), 0);

}

void CMinionHPRatioScript::tick()
{
	if (!GetOwner()->GetParent()->IsDead())
	{
		CGameObject* ParentObj = GetOwner()->GetParent();
		bool IsCulling = ParentObj->GetRenderComponent()->IsCulled();
		bool IsRaySightCulling = ParentObj->GetRenderComponent()->IsUsingRaySightCulling();
		if (IsCulling && IsRaySightCulling)
			return;

		m_fCurHP = ParentObj->GetScript<CUnitScript>()->GetCurHP();
		m_fTotalHP = ParentObj->GetScript<CUnitScript>()->GetMaxHP();
		Transform()->SetAbsolute(false);
		Transform()->SetBillBoard(true);
		//============================================================================
		// 현재 자식은 직교투영이고, 부모는 원근투영이라서 문제가 됨. 자식이 부모의 Pos,Scale,Rot를 가져와서 자신의 행렬에 적용시키는데 
		// 이값들은 원근 기준의 좌표이기때문에 원근상의 좌표가 직교투영에 계속 영향을 줘서 그냥똑같이 월드상에 배치해야 할것같음.
		//============================================================================
		//----------HP------------
		
		m_fRatio = m_fCurHP / m_fTotalHP;
		if (m_fCurHP >= m_fTotalHP)
			m_fCurHP = m_fTotalHP;

		if (m_fCurHP <= 0.f)
			m_fCurHP = 0.f;
		//-------------------------

		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fRatio);
	}

}

void CMinionHPRatioScript::BeginOverlap(CCollider2D* _Other)
{
}


CMinionHPRatioScript::CMinionHPRatioScript()
	:CScript((UINT)SCRIPT_TYPE::MINIONHPRATIOSCRIPT)
{
}

CMinionHPRatioScript::~CMinionHPRatioScript()
{
}
