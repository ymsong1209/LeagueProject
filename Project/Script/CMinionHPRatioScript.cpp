#include "pch.h"
#include "CMinionHPRatioScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CEngine.h>
#include "CUnitScript.h"

void CMinionHPRatioScript::begin()
{
	GetOwner()->ChangeLayer(31);
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Transform()->SetNoParentaffected(true);
	OffsetBarPos = Vec2(0.f, 60.f);

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

		Transform()->SetAbsolute(false);
		Transform()->SetRelativeScale(Vec3(66.f, 7.f, 1.f));

		//----------HP------------
		//======디버깅용=========
		//m_fCurHP = 570.f;
		//m_fTotalHP = 1000.f;
		//====================
		m_fRatio = m_fCurHP / m_fTotalHP;
		if (m_fCurHP >= m_fTotalHP)
			m_fCurHP = m_fTotalHP;

		if (m_fCurHP <= 0.f)
			m_fCurHP = 0.f;
		//-------------------------
		Vec3 Pos = ParentObj->Transform()->GetRelativePos();
		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		Matrix viewmat = MainCam->GetViewMat();
		Matrix projmat = MainCam->GetProjMat();
		Matrix MatViewProj = viewmat * projmat;

		// 4D 벡터로 변환 (w 요소를 1로 설정)
		Vec4 Pos4 = Vec4(Pos.x, Pos.y, Pos.z, 1.0f);
		// MatViewProj에 곱해줌
		Vec4 ProjPos = Pos4.Transform(Pos4, MatViewProj);
		// w로 나눠서 클리핑(ndc) 좌표계로 변환
		ProjPos /= ProjPos.w;
		Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();  //화면 해상도를 가져옴
		Vec2 ObjscreenPos = ((Vec2(ProjPos.x, ProjPos.y) + Vec2(1.f, 1.f)) / Vec2(2.f, 2.f)) * Resolution;

		Vec4 ndcVec = Vec4((2.0f * ObjscreenPos.x) / Resolution.x - 1.0f, 1.0f - (2.0f * ObjscreenPos.y) / Resolution.y, 1.f, 1.f);

		Matrix viewInvMatrix = UICamera->GetViewMatInv();
		Matrix projInvMatrix = UICamera->GetProjMatInv();
		Matrix invViewProjMatrix = viewInvMatrix * projInvMatrix;

		Vec3 worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);
		Vec3 FinalPos = Vec3(worldVec.x + OffsetBarPos.x, -worldVec.y + OffsetBarPos.y, 700.f);
		Transform()->SetRelativePos(FinalPos);

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
