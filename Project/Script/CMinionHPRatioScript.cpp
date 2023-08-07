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
	/*CGameObject* ParentObj = GetOwner()->GetParent();
	Faction OwnerFaction = ParentObj->GetScript<CUnitScript>()->GetFaction();
	if (OwnerFaction == Faction::BLUE)
	{
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionBar.mtrl"), 0);
		MeshRender()->GetDynamicMaterial(0);
	}
	else if (OwnerFaction == Faction::RED)
	{
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedMinionBar.mtrl"), 0);
		MeshRender()->GetDynamicMaterial(0);
	}*/

	//===================인스턴싱 모드==========================
	// 
	CGameObject* ParentObj = GetOwner()->GetParent();
	Faction OwnerFaction = ParentObj->GetScript<CUnitScript>()->GetFaction();
	if (OwnerFaction == Faction::BLUE)
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BlueMinionBarHP.mtrl"), 0);
	else if (OwnerFaction == Faction::RED)
		MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RedMinionBarHP.mtrl"), 0);
	
}

void CMinionHPRatioScript::tick()
{
	//if (GetOwner()->GetParent() && !GetOwner()->GetParent()->IsDead())
	{
		Transform()->SetAbsolute(false);
		Transform()->SetRelativePos(Vec3(0.f, 100.f, 50.f));
		Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
		//============================================================================
		// 현재 자식은 직교투영이고, 부모는 원근투영이라서 문제가 됨. 자식이 부모의 Pos,Scale,Rot를 가져와서 자신의 행렬에 적용시키는데 
		// 이값들은 원근 기준의 좌표이기때문에 원근상의 좌표가 직교투영에 계속 영향을 줘서... 그냥똑같이 월드상에 배치해야 할것같음.
		//============================================================================
		
		////부모의위치를 화면상의 위치로 전환 -> 부모위치의 화면상의 위치가 나옴.
		//CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		//Matrix viewmat = MainCam->GetViewMat();
		//Matrix projmat = MainCam->GetProjMat();
		//Matrix MatViewProj = viewmat * projmat;

		//// 4D 벡터로 변환 (w 요소를 1로 설정)
		//Vec4 Pos4 = Vec4(Pos.x, Pos.y, Pos.z, 1.0f);
		//// MatViewProj에 곱해줌
		//Vec4 ProjPos = Pos4.Transform(Pos4, MatViewProj);
		//// w로 나눠서 클리핑(ndc) 좌표계로 변환
		//ProjPos /= ProjPos.w;
		//Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();  //화면 해상도를 가져옴
		//Vec2 screenSize(Resolution.x, Resolution.y);
		//Vec2 ObjscreenPos = ((Vec2(ProjPos.x, ProjPos.y) + Vec2(1.f, 1.f)) / Vec2(2.f, 2.f)) * screenSize;
		//Vec4 ndcVec = Vec4((2.0f * ObjscreenPos.x) / Resolution.x - 1.0f, 1.0f - (2.0f * ObjscreenPos.y) / Resolution.y, 1.f, 1.f);

		//// Get the inverse of the view-projection matrix
		//Matrix viewInvMatrix = UICamera->GetViewMatInv();
		//Matrix projInvMatrix = UICamera->GetProjMatInv();
		//Matrix invViewProjMatrix = viewInvMatrix * projInvMatrix;

		//// Transform to world coordinates
		//Vec3 worldVec = XMVector3TransformCoord(ndcVec, invViewProjMatrix);
		//Vec3 OffsetPos = Vec3(-3.f, 117.f, 0.f);
		//Vec3 FinalPos = Vec3(worldVec.x, -worldVec.y, 700.f);
		//Transform()->SetRelativePos(FinalPos);
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
