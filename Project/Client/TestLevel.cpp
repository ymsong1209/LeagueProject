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
#include <Script\CCameraMoveScript.h>
#include <Engine\CPathFindMgr.h>


#include "CLevelSaveLoad.h"
#include <Script/CScriptMgr.h>


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


	CGameObject* LoLMapCollider = new CGameObject;
	LoLMapCollider->SetName(L"LoLMapCollider");
	LoLMapCollider->AddComponent(new CTransform);
	LoLMapCollider->AddComponent(new CCollider2D);
	LoLMapCollider->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	LoLMapCollider->Collider2D()->SetOffsetScale(Vec2(2700.f, 2700.f));
	LoLMapCollider->Collider2D()->SetOffsetPos(Vec3(1125.f, 16.f, 1200.f));
	LoLMapCollider->Transform()->SetGizmoObjExcept(true);
	SpawnGameObject(LoLMapCollider, Vec3(0.f, 0.f, 0.f), 6);


	// ============
	// FBX Loading
	// ============	
	{
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;
		//
		//pMeshData = nullptr;
		//pObj = nullptr;
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"Jinx");
		//pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		//pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->AddComponent(new CPlayerScript);
		//pObj->AddComponent(new CPathFinder);
		//pObj->AddComponent(new CCollider3D);
		//
		//pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		//pObj->Collider3D()->SetAbsolute(true);
		//pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		//pObj->Collider3D()->SetDrawCollision(false);
		//pObj->Animator3D()->SetRepeat(true);
		//pObj->Animator3D()->Play(L"Jinx\\Idle1_Base", true, 0.1f);
		//pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		//
		//pObj->Transform()->SetUseMouseOutline(true);
		//
		//SpawnGameObject(pObj, Vec3(0, 0, 0), 0);
	}


	// LoLMap 로딩
	{
	
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		for (int i = 0; i <=25; ++i) {
			wstring num = std::to_wstring(i);
			wstring FBXFilePath = L"fbx\\land";
			FBXFilePath += num;
			FBXFilePath += L".fbx";
			
			wstring FBXFileName = L"land";
			FBXFileName += num;

			pMeshData = CResMgr::GetInst()->LoadFBX(FBXFilePath);
			pObj = pMeshData->Instantiate();
			pObj->SetName(FBXFileName);
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetShowDebugBound(false);
			pObj->Transform()->SetGizmoObjExcept(false);
			SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 6);
		}
	}


	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CreateLoginLevel()
{
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
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
	SpawnGameObject(pMainCam, Vec3(1386.f, 498.f, -9.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));

	pLightObj->Light3D()->SetLightDiffuse(Vec3(0.5f, 0.5f, 0.5f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.9f, 0.9f, 0.9f));


	pLightObj->Light3D()->SetRadius(400.f);

	SpawnGameObject(pLightObj, -pLightObj->Light3D()->GetLightDirection() * 1000.f, 0);


	// SkyBox
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"Login_SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->Transform()->SetRelativeRot(Vec3(0.f, XM_PI / 2.f, 0.f));
	pSkyBox->Transform()->SetGizmoObjExcept(true);
	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyDawn.dds"));
	pSkyBox->SkyBox()->SetRaySightCulling(false);

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);

	// Anim Test Object
	//CGameObject* pAnimTestObj = new CGameObject;
	//pAnimTestObj->SetName(L"AnimTestObj");
	//pAnimTestObj->AddComponent(new CMeshRender);
	//pAnimTestObj->AddComponent(new CAnimator2D);
	//pAnimTestObj->AddComponent(new CTransform);
	//pAnimTestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pAnimTestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	//pAnimTestObj->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	//pAnimTestObj->Animator2D()->SetCurAnim(CResMgr::GetInst()->FindRes<CAnim2D>(L"LOGIN_LEFT").Get());
	//SpawnGameObject(pAnimTestObj, Vec3(0.f, 0.f, 50.f), 0.f);


	
	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CreateFactionLevel()
{
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
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
	SpawnGameObject(pMainCam, Vec3(1386.f, 498.f, -9.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));

	pLightObj->Light3D()->SetLightDiffuse(Vec3(0.5f, 0.5f, 0.5f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.9f, 0.9f, 0.9f));


	pLightObj->Light3D()->SetRadius(400.f);

	SpawnGameObject(pLightObj, -pLightObj->Light3D()->GetLightDirection() * 1000.f, 0);


	// SkyBox
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"Faction_SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->Transform()->SetRelativeRot(Vec3(0.f, XM_PI / 2.f, 0.f));
	pSkyBox->Transform()->SetGizmoObjExcept(true);
	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));
	pSkyBox->SkyBox()->SetRaySightCulling(false);

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CreateChampionPickLevel()
{
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
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
	SpawnGameObject(pMainCam, Vec3(1386.f, 498.f, -9.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));

	pLightObj->Light3D()->SetLightDiffuse(Vec3(0.5f, 0.5f, 0.5f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.9f, 0.9f, 0.9f));


	pLightObj->Light3D()->SetRadius(400.f);

	SpawnGameObject(pLightObj, -pLightObj->Light3D()->GetLightDirection() * 1000.f, 0);


	// SkyBox
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"Champion_SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->Transform()->SetRelativeRot(Vec3(0.f, XM_PI / 2.f, 0.f));
	pSkyBox->Transform()->SetGizmoObjExcept(true);
	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyDawn.dds"));
	pSkyBox->SkyBox()->SetRaySightCulling(false);

	SpawnGameObject(pSkyBox, Vec3(0.f, 0.f, 0.f), 0);

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CreateIngameLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);

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
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));
	SpawnGameObject(pMainCam, Vec3(1386.f, 498.f, -9.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));

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


	CGameObject* LoLMapCollider = new CGameObject;
	LoLMapCollider->SetName(L"LoLMapCollider");
	LoLMapCollider->AddComponent(new CTransform);
	LoLMapCollider->AddComponent(new CCollider2D);
	LoLMapCollider->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
	LoLMapCollider->Collider2D()->SetOffsetScale(Vec2(2700.f, 2700.f));
	LoLMapCollider->Collider2D()->SetOffsetPos(Vec3(1125.f, 16.f, 1200.f));
	LoLMapCollider->Transform()->SetGizmoObjExcept(true);
	SpawnGameObject(LoLMapCollider, Vec3(0.f, 0.f, 0.f), 6);


	// ============
	// FBX Loading
	// ============	
	{
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;
		//
		//pMeshData = nullptr;
		//pObj = nullptr;
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"Jinx");
		//pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		//pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->AddComponent(new CPlayerScript);
		//pObj->AddComponent(new CPathFinder);
		//pObj->AddComponent(new CCollider3D);
		//
		//pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		//pObj->Collider3D()->SetAbsolute(true);
		//pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		//pObj->Collider3D()->SetDrawCollision(false);
		//pObj->Animator3D()->SetRepeat(true);
		//pObj->Animator3D()->Play(L"Jinx\\Idle1_Base", true, 0.1f);
		//pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		//
		//pObj->Transform()->SetUseMouseOutline(true);
		//
		//SpawnGameObject(pObj, Vec3(0, 0, 0), 0);
	}


	// LoLMap 로딩
	{

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		for (int i = 0; i <= 25; ++i) {
			wstring num = std::to_wstring(i);
			wstring FBXFilePath = L"fbx\\land";
			FBXFilePath += num;
			FBXFilePath += L".fbx";

			wstring FBXFileName = L"land";
			FBXFileName += num;

			pMeshData = CResMgr::GetInst()->LoadFBX(FBXFilePath);
			pObj = pMeshData->Instantiate();
			pObj->SetName(FBXFileName);
			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetShowDebugBound(false);
			pObj->Transform()->SetGizmoObjExcept(false);
			SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 6);
		}
	}

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}
