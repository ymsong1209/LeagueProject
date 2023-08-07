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
	//===================�ν��Ͻ� ���XXX==========================
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

	//===================�ν��Ͻ� ���==========================
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
		// ���� �ڽ��� ���������̰�, �θ�� ���������̶� ������ ��. �ڽ��� �θ��� Pos,Scale,Rot�� �����ͼ� �ڽ��� ��Ŀ� �����Ű�µ� 
		// �̰����� ���� ������ ��ǥ�̱⶧���� ���ٻ��� ��ǥ�� ���������� ��� ������ �༭... �׳ɶȰ��� ����� ��ġ�ؾ� �ҰͰ���.
		//============================================================================
		
		////�θ�����ġ�� ȭ����� ��ġ�� ��ȯ -> �θ���ġ�� ȭ����� ��ġ�� ����.
		//CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		//Matrix viewmat = MainCam->GetViewMat();
		//Matrix projmat = MainCam->GetProjMat();
		//Matrix MatViewProj = viewmat * projmat;

		//// 4D ���ͷ� ��ȯ (w ��Ҹ� 1�� ����)
		//Vec4 Pos4 = Vec4(Pos.x, Pos.y, Pos.z, 1.0f);
		//// MatViewProj�� ������
		//Vec4 ProjPos = Pos4.Transform(Pos4, MatViewProj);
		//// w�� ������ Ŭ����(ndc) ��ǥ��� ��ȯ
		//ProjPos /= ProjPos.w;
		//Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();  //ȭ�� �ػ󵵸� ������
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
