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
#include <Script\CMonsterScript.h>

#include "CLevelSaveLoad.h"



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
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	
	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));


	pLightObj->Light3D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.3f, 0.3f, 0.3f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.15f, 0.15f, 0.15f));	

	pLightObj->Light3D()->SetRadius(400.f);

	SpawnGameObject(pLightObj, -pLightObj->Light3D()->GetLightDirection() * 1000.f, 0);


	// SkyBox
	CGameObject* pSkyBox  = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->Transform()->SetRelativeRot(Vec3(0.f, XM_PI / 2.f, 0.f));

	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);


	// Shadow Test Object
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"Sphere Object");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 300.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"),0);
	pObject->MeshRender()->SetDynamicShadow(true);
	pObject->MeshRender()->SetBounding(150.f);

	SpawnGameObject(pObject, Vec3(0.f, 200.f, 300.f), 0);

	// LandScape Object
	CGameObject* pLandScape = new CGameObject;
	pLandScape->SetName(L"LandScape");

	pLandScape->AddComponent(new CTransform);
	pLandScape->AddComponent(new CLandScape);
	
	pLandScape->Transform()->SetRelativeScale(Vec3(200.f, 1000.f, 200.f));

	pLandScape->LandScape()->SetFace(32, 32);
	pLandScape->LandScape()->SetFrustumCheck(false);
	pLandScape->LandScape()->SetHeightMap(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\HeightMap_01.jpg"));
	pLandScape->LandScape()->SetDynamicShadow(true);

	SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 0);


	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
		SpawnGameObject(pMeshData->Instantiate(), Vec3(0.f, 500.f, 0.f), 0);
	}

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");	
}
