#include "pch.h"
#include "CMouseCursorUIScript.h"
#include <Engine\CKeyMgr.h>
#include <Engine\CEngine.h>
#include <Engine\CRenderMgr.h>
#include "CMoveCursorUIScript.h"

void CMouseCursorUIScript::begin()
{
	//CGameObject* MouseObj = new CGameObject;
	//MouseObj->AddComponent(new CTransform);
	//MouseObj->AddComponent(new CMeshRender);
	//MouseObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//MouseObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L""), 0);
	//MouseObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	//SpawnGameObject(MouseObj, Vec3(100.f, 100.f, 100.f),31);

	Mouse = new CGameObject;
	Mouse->SetName(L"Mouse");
	Mouse->AddComponent(new CTransform);
	Mouse->AddComponent(new CMeshRender);
	Mouse->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Mouse->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MouseCursor.mtrl"), 0);
	Mouse->Transform()->SetRelativeScale(Vec3(30.f, 30.f, 2.f));
	SpawnGameObject(Mouse, Vec3(1.f, 1.f, 1.f), 31);
}


void CMouseCursorUIScript::tick()
{
	Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();
	Vec2 Pos = CKeyMgr::GetInst()->GetMousePos();
	if (Mouse)
	{
		CTransform* Transform = Mouse->Transform();
		Vec3 Scale = Transform->GetRelativeScale();
		Scale /= 2.f;
		Transform->SetRelativePos(Pos.x - (Resolution.x / 2) + Scale.x, -Pos.y + (Resolution.y / 2) - Scale.y, 1.f);
	}

	//=======================
	// 마우스 커서 숨기기
	if (KEY_TAP(KEY::TAB))
	{
		if (!m_bOnlyMouseCursorOn)
		{
			ShowCursor(FALSE);
			m_bOnlyMouseCursorOn = true;
		}
		else
		{
			ShowCursor(TRUE);
			m_bOnlyMouseCursorOn = false;
		}
	}
	//=======================

	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();

	if (KEY_TAP(KEY::RBTN))
	{
		CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"LoLMapCollider");
		IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);

		if (result.bResult)
		{
			if (!MoveArrow)
			{
				Ptr<CMeshData> pMeshData = nullptr;
				CGameObject* pObj = nullptr;
				pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\arrow.fbx");
				pObj = pMeshData->Instantiate();
				pObj->SetName(L"arrow");
				pObj->GetRenderComponent()->SetFrustumCheck(false);
				pObj->AddComponent(new CMoveCursorUIScript);
				pObj->Transform()->SetRelativeScale(Vec3(0.25f, 0.25f, 0.25f));
				pObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f,0.f));
				SpawnGameObject(pObj, result.vCrossPoint, 0);
				MoveArrow = pObj;
			}
		}
	}


	if (MoveArrow && MoveArrow->IsDead())
	{
		MoveArrow = nullptr;
	}

}

void CMouseCursorUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CMouseCursorUIScript::CMouseCursorUIScript()
	:CScript((UINT)SCRIPT_TYPE::MOUSECURSORUISCRIPT)
	, m_bOnlyMouseCursorOn(false)
{

}

CMouseCursorUIScript::~CMouseCursorUIScript()
{
}
