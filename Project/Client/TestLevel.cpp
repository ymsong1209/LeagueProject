#include "pch.h"
#include "TestLevel.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CCameraMoveScript.h>
#include <Engine\CPathFindMgr.h>


#include "CLevelSaveLoad.h"
#include <Script/CScriptMgr.h>
#include <Script/CJinxScript.h>
#include <Script/CVayneScript.h>


void CreateTestLevel()
{
	//return;	

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"LoLMap");
	//롤맵 레이어에는 롤맵만 넣을것!
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f),0.f, 0.f));
	SpawnGameObject(pMainCam, Vec3(1386.f, 498.f, -9.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));



	//pLightObj->Light3D()->SetLightDiffuse(Vec4(1.f, 1.f, 1.f,1.f));
	//pLightObj->Light3D()->SetLightSpecular(Vec4(0.f, 0.f, 0.f,1.f));
	//pLightObj->Light3D()->SetLightAmbient(Vec4(1.f, 1.f, 1.f,1.f));

	pLightObj->Light3D()->SetLightDiffuse(Vec3(0.5f, 0.5f, 0.5f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.9f, 0.9f, 0.9f));


	pLightObj->Light3D()->SetRadius(400.f);

	SpawnGameObject(pLightObj, -pLightObj->Light3D()->GetLightDirection() * 1000.f, 0);


	// SkyBox
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->Transform()->SetRelativeRot(Vec3(0.f, XM_PI / 2.f, 0.f));
	pSkyBox->Transform()->SetGizmoObjExcept(true);
	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));
	pSkyBox->SkyBox()->SetRaySightCulling(false);

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);


	//// Shadow Test Object
	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Sphere Object");
	//
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	////pObject->AddComponent(new CAnimator2D);
	//
	//pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));
	//
	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	//pObject->MeshRender()->SetDynamicShadow(true);
	//pObject->MeshRender()->SetBounding(150.f);
	//
	//SpawnGameObject(pObject, Vec3(0.f, 200.f, 300.f), 0);

	// Anim2d Test Object
	//CGameObject* pObject3 = new CGameObject;
	//pObject3->SetName(L"Sphere Object");
	//
	//pObject3->AddComponent(new CTransform);
	//pObject3->AddComponent(new CMeshRender);
	//pObject3->AddComponent(new CAnimator2D);
	//pObject3->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));
	//
	//pObject3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject3->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	//pObject3->MeshRender()->SetDynamicShadow(true);
	//pObject3->MeshRender()->SetBounding(150.f);
	//
	//SpawnGameObject(pObject3, Vec3(0.f, 200.f, 300.f), 0);




	// LandScape Object
	//CGameObject* pLandScape = new CGameObject;
	//pLandScape->SetName(L"LandScape");


	//pLandScape->AddComponent(new CTransform);
	//pLandScape->AddComponent(new CLandScape);

	//pLandScape->Transform()->SetRelativeScale(Vec3(200.f, 1000.f, 200.f));

	//pLandScape->LandScape()->SetFace(32, 32);
	//pLandScape->LandScape()->SetFrustumCheck(false);

	//SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 0);

	

	CGameObject* LoLMapCollider = new CGameObject;
	LoLMapCollider->SetName(L"LoLMapCollider");
	LoLMapCollider->AddComponent(new CTransform);
	LoLMapCollider->AddComponent(new CCollider2D);
	LoLMapCollider->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	LoLMapCollider->Collider2D()->SetOffsetScale(Vec2(2700.f, 2700.f));
	LoLMapCollider->Collider2D()->SetOffsetPos(Vec3(1125.f, 16.f, 1200.f));
	LoLMapCollider->Transform()->SetGizmoObjExcept(true);
	SpawnGameObject(LoLMapCollider, Vec3(0.f, 0.f, 0.f), 6);


	// ===========
	// Vayne Test
	// ===========
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Vayne.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Vayne");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Vayne");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->AddComponent(new CPlayerScript);
		pObj->AddComponent(new CVayneScript);
		pObj->AddComponent(new CPathFinder);

		pObj->Animator3D()->PlayRepeat(L"Vayne\\Attack1.002", true, true, 0.1f);
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

		pObj->Transform()->SetUseMouseOutline(false);
		
		auto b = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png");
		CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Vayne_Vehicle_Mtrl.mtrl")->SetTexParam(TEX_0, b);
		int c = pObj->MeshRender()->GetMtrlCount();

		SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
	}


	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Jinx");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->AddComponent(new CPlayerScript);
		//pObj->AddComponent(new CJinxScript);
		pObj->AddComponent(new CPathFinder);
		pObj->AddComponent(new CCollider3D);

		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pObj->Collider3D()->SetAbsolute(true);
		pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		pObj->Collider3D()->SetDrawCollision(false);
		pObj->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true,true,0.1f);
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));

		pObj->Transform()->SetUseMouseOutline(true);

		SpawnGameObject(pObj, Vec3(0, 0, 0), 0);


		//-------------------------------넥서스-----------------------------------------
		//넥서스는 0번머터리얼을 쓰면 1번 머터리얼에는 알파텍스쳐를 장착하고, 1번머터리얼을 쓰면 0번머터리얼에 알파 텍스쳐를 장착해줘야한다.
		//-----터지는 넥서스쪽 보기 ------ 
		//pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\nexus_destroyed_red_clear.png"));
		//pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));

		//-----빙빙 도는 넥서스쪽 보기------ 
		/*pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\nexus_red_clear.png"));*/
		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\nexus.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"blue_nexus");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\nexus");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"nexus\\sruap_order_idle.anm_skinned_mesh.001", true,true,0.1f);
		pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_ordernexus_tx_cm_clear.png"));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(229.7f, 15.9f, 241.5f), 0);

		//------------------------------------------------------------------------------

	
		//억제기 평상시 애니메이션은 idle1 애니메이션임!! 
		//억제기의 평상시에는 1번에 alphaTex 이미지를 넣어주고, 0번 머터리얼에 억제기 기본 머터리얼을 넣어주면됨(기본적으로 되어있어서 따로 세팅해줄 필요는 없음)
		//억제기가 폭발할때는 0번머터리얼에 alphaTex 이미지를 넣어주고, 1번머터리얼에 억제기 전용 detroy텍스쳐를 입혀주면됨 (따로 세팅해줘야함)
		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"blue_Inhibitor");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true,true, 0.1f);
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-45.48f), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(537.71f, 14.2f, 546.9f), 0);

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"blue_Inhibitor2");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001",true, true, 0.1f);
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-89.48f), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(169.86f, 14.2f, 527.02f), 0);

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"blue_Inhibitor2");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true,true, 0.1f);
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(2.f), 0.f));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(634.97f, 14.2f, 183.08f), 0);

		//----------RED-------------------------------

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\nexus.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"red_nexus");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\nexus");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"nexus\\sruap_order_idle.anm_skinned_mesh.001", true,true, 0.1f);
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Mat_Red.mtrl"), 1);
		pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(1952.174f, 15.26f, 1956.22f), 0);

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"red_Inhibitor1");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true,true, 0.1f);
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(0.f), XMConvertToRadians(-180.f)));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(1661.7f, 14.8f, 2013.9f), 0);

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"red_Inhibitor2");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true,true, 0.1f);
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(45.f), XMConvertToRadians(-180.f)));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(1711.f, 14.8f, 1721.f), 0);

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"red_Inhibitor3");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
		pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
		pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(90.f), XMConvertToRadians(-180.f)));
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		SpawnGameObject(pObj, Vec3(2006.9f, 14.8f, 1670.1f), 0);

	}


	// LoLMap 로딩
	{
	
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		for (int i = 0; i <= 25; ++i) 
		{
			wstring num = std::to_wstring(i);
			wstring FBXFilePath = L"fbx\\land";
			FBXFilePath += num;
			FBXFilePath += L".fbx";

			wstring FBXFileName = L"land";
			FBXFileName += num;

			pMeshData = CResMgr::GetInst()->LoadFBX(FBXFilePath);
			pObj = pMeshData->Instantiate();
			pObj->SetName(FBXFileName);

			//맵이 다 (0,0,0) 기준 컬링인거같아 불편해서 잠시 끕니다.
			pObj->GetRenderComponent()->SetFrustumCheck(false);

			pObj->GetRenderComponent()->SetShowDebugBound(false);
			pObj->Transform()->SetGizmoObjExcept(false);
			SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 6);
		}
	}


	// TestFastForward
	CGameObject* pRectFast = new CGameObject;
	pRectFast->SetName(L"ReftFast");
	pRectFast->AddComponent(new CMeshRender);
	pRectFast->AddComponent(new CTransform);
	pRectFast->AddComponent(new CCollider2D);

	pRectFast->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));
	pRectFast->Transform()->SetUseMouseOutline(true);

	pRectFast->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pRectFast->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	pRectFast->MeshRender()->GetDynamicMaterial(0);
	pRectFast->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));

	pRectFast->Collider2D()->SetAbsolute(false);
	pRectFast->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	pRectFast->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f,0.f));
	pRectFast->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);

	SpawnGameObject(pRectFast, Vec3(-600.f, 0.f, 500.f), 0);


	// MiniMap
	CGameObject* MiniMap = new CGameObject;
	MiniMap->SetName(L"MiniMap");
	MiniMap->AddComponent(new CMeshRender);
	MiniMap->AddComponent(new CTransform);

	MiniMap->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));
	MiniMap->Transform()->SetUseMouseOutline(false);
	
	MiniMap->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MiniMap->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MiniMapMtrl"), 0);
	MiniMap->MeshRender()->GetDynamicMaterial(0);
	MiniMap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\MiniMap.dds"));
	MiniMap->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"FogFilterMap"));


	SpawnGameObject(MiniMap, Vec3(-600.f, 300.f, 700.f), 0);




	// Ray Test Object1
	CGameObject* RayTestObj1 = new CGameObject;
	RayTestObj1->SetName(L"RayTestObj1");

	RayTestObj1->AddComponent(new CMeshRender);
	RayTestObj1->AddComponent(new CTransform);
	RayTestObj1->AddComponent(new CCollider2D);

	RayTestObj1->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));
	RayTestObj1->Transform()->SetUseMouseOutline(true);
	//RayTestObj1->MeshRender()->SetUsingMovingVec(true);

	RayTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	RayTestObj1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	RayTestObj1->MeshRender()->GetDynamicMaterial(0);
	RayTestObj1->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"));

	RayTestObj1->Collider2D()->SetAbsolute(false);
	RayTestObj1->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	RayTestObj1->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f,0.f));
	RayTestObj1->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);


	SpawnGameObject(RayTestObj1, Vec3(-600.f, 0.f, 600.f), 0);

	// Ray Cube Test Object 1
	CGameObject* RayCubeTestObj1 = new CGameObject;
	RayCubeTestObj1->SetName(L"RayCubeTestObj1");

	RayCubeTestObj1->AddComponent(new CMeshRender);
	RayCubeTestObj1->AddComponent(new CTransform);
	RayCubeTestObj1->AddComponent(new CCollider3D);
	RayCubeTestObj1->Transform()->SetUseMouseOutline(true);

	RayCubeTestObj1->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	RayCubeTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	RayCubeTestObj1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	RayCubeTestObj1->MeshRender()->GetDynamicMaterial(0);

	RayCubeTestObj1->Collider3D()->SetAbsolute(false);
	RayCubeTestObj1->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	RayCubeTestObj1->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	RayCubeTestObj1->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);


	SpawnGameObject(RayCubeTestObj1, Vec3(-600.f, -450.f, 700.f), 0);


	// Ray Cube Test Object 2
	CGameObject* RayCubeTestObj2 = new CGameObject;
	RayCubeTestObj2->SetName(L"RayCubeTestObj2");

	RayCubeTestObj2->AddComponent(new CMeshRender);
	RayCubeTestObj2->AddComponent(new CTransform);
	RayCubeTestObj2->AddComponent(new CCollider3D);
	RayCubeTestObj2->Transform()->SetUseMouseOutline(true);

	RayCubeTestObj2->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	RayCubeTestObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	RayCubeTestObj2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	RayCubeTestObj2->MeshRender()->GetDynamicMaterial(0);

	RayCubeTestObj2->Collider3D()->SetAbsolute(false);
	RayCubeTestObj2->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	RayCubeTestObj2->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	RayCubeTestObj2->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);


	SpawnGameObject(RayCubeTestObj2, Vec3(-600.f, -450.f, 960.f), 0);


	// TestFastForward
	//CGameObject* pRectFast2 = new CGameObject;
	//pRectFast2->SetName(L"MoveSphere");
	//pRectFast2->AddComponent(new CMeshRender);
	//pRectFast2->AddComponent(new CTransform);
	//pRectFast2->AddComponent(new CCollider3D);
	//pRectFast2->Transform()->SetUseMouseOutline(true);

	//pRectFast2->Transform()->SetRelativeScale(Vec3(45.f, 45.f, 45.f));
	//pRectFast2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	//pRectFast2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);
	//pRectFast2->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
	//pRectFast2->Collider3D()->SetAbsolute(false);
	//pRectFast2->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	//pRectFast2->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	//SpawnGameObject(pRectFast2, Vec3(0.f, 0.f, 0.f), 0);



	// Anim Test Object
	//CGameObject* pAnimTestObj = new CGameObject;
	//pAnimTestObj->SetName(L"AnimTestObj");

	//pAnimTestObj->AddComponent(new CMeshRender);
	//pAnimTestObj->AddComponent(new CAnimator2D);
	//pAnimTestObj->AddComponent(new CTransform);

	//pAnimTestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pAnimTestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);

	//pAnimTestObj->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));

	// 테스트를 해보고 싶은데 FindRes 에 CAnim2D가 없어서 못해봤습니다..
	// 추가할라고 마음 먹으면 할 수 있을 것 같긴한데, 너무 크게 바꾸는 것 같아 해보진 않았습니다.
	//pAnimTestObj->Animator2D()->SetCurAnim(CResMgr::GetInst()->FindRes<CAnim2D>(L"Link_Basic").Get());


	//SpawnGameObject(pAnimTestObj, Vec3(0.f, 0.f, 0.f), 0.f);


		
	



	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}
