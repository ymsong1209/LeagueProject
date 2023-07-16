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
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(45.f), 0.f, 0.f));
	SpawnGameObject(pMainCam, Vec3(0.f, 480.f, 0.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight3D);

	pLightObj->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light3D()->SetLightDirection(Vec3(1.f, -1.f, 1.f));


	pLightObj->Light3D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(1.f, 1.f, 1.f));

	pLightObj->Light3D()->SetRadius(400.f);

	SpawnGameObject(pLightObj, -pLightObj->Light3D()->GetLightDirection() * 1000.f, 0);


	// SkyBox
	CGameObject* pSkyBox = new CGameObject;
	pSkyBox->SetName(L"SkyBox");

	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));
	pSkyBox->Transform()->SetRelativeRot(Vec3(0.f, XM_PI / 2.f, 0.f));

	pSkyBox->SkyBox()->SetType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->SetSkyTexture(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\skybox\\SkyWater.dds"));

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



	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\LoLMapRot19Size30.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"LoLMapRot19Size30");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);

		// 마우스 피킹 이동을 위한 맵 콜리전 - 따로 오브젝트로 빼준 이유는 렉트메쉬콜리전을 회전시켜야하는데 맵도 같이 회전되면 안돼서 따로 오브젝트로 뺐음
		// 맵은 이동된다는 가정을 하지않음, 크기,회전,이동을 해버리면 네브메쉬랑 좌표값이 안맞게 되어버린다.
		// 맵이 있으면 맵 콜리전도 있어야함!
		CGameObject* MapCollision = new CGameObject;
		MapCollision->SetName(L"MapCollision");
		MapCollision->AddComponent(new CCollider2D);
		MapCollision->AddComponent(new CTransform);
		MapCollision->Collider2D()->SetAbsolute(true);
		MapCollision->Collider2D()->SetOffsetScale(Vec2(2700.f, 2700.f));
		MapCollision->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		SpawnGameObject(MapCollision, Vec3(1125.f, 16.f, 1200.f), 0);
		CPathFindMgr::GetInst()->SetMapCollision(MapCollision); //마우스 피킹 진행할 맵 콜리전으로 지정

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"jungle_blue");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Animator3D()->Play(L"jungle_blue-jungle_blue_AllAnim", true, 0.5f);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));
		SpawnGameObject(pObj, Vec3(190.f, 0.f, 607.f), 0);

		pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Jinx");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->Animator3D()->Play(L"jungle_blue-jungle_blue_AllAnim", true, 0.5f);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));
		SpawnGameObject(pObj, Vec3(0, 0, 0), 0);


		//단일재생
		//pObj->Animator3D()->Play(L"Take 001");
		//단일재생, 이전 애니메이션 0.5초 블렌딩 후 단일재생
		//pObj->Animator3D()->Play(L"Take001", true, 0.5f);
		//반복재생, 마지막프레임후 첫프레임으로 blend안함
		//pObj->Animator3D()->Play(L"Take001", true, false);
		//반복재생, AnimA->Take001로 0.5초동안 애니메이션 blend하고 take001재생,take001마지막프레임->첫프레임 돌아오면서 blend안하기 
		//pObj->Animator3D()->Play(L"Take001", true, false, true, 0.5f);
		//반복재생, AnimA->Take001로 0.5초동안 애니메이션 blend하고 take001재생,take001마지막프레임->첫프레임 돌아오면서 blend 0.5초동안 하기
		//pObj->Animator3D()->Play(L"Take001", true, true, true, 0.5f);
		//사용 예시

		//IdleAnimation 반복재생 blend안함
		//pObj->animator3d()->play(L"IdleAnimation",true,false);
		//공격버튼 누르면 공격 애니메이션 단일재생, blending 0.2초
		//if(key_pressed(key::lbtn){
		//changestate(attack);
		//pobj->animator3d()->play(L"AttackAnimation", true,0.2f);
		//}
		//공격 애님 끝나면 다시 idle로 전환
		//if(animator3d()->getcuranim()->isfinish()){
		//changestate(idle)
		////Attack->Idle 0.2초 blend주고, idle animation 재생, idle anim은 마지막프레임->첫프레임 이동시 blend없음 
		//pobj->animator3d()->play(L"IdleAnimation",true,false,true,0.2f); 
		//}




		//SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
	}

	//CGameObject* pObj = new CGameObject;
	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	////pObj->AddComponent(new CAnimator3D);
	////pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\jinx55");
	//
	//pObj->SetName(L"jinx");
	//SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);


	// TestFastForward
	CGameObject* pRectFast = new CGameObject;
	pRectFast->SetName(L"ReftFast");
	pRectFast->AddComponent(new CMeshRender);
	pRectFast->AddComponent(new CTransform);
	pRectFast->AddComponent(new CCollider2D);

	pRectFast->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));

	pRectFast->MeshRender()->SetUsingMovingVec(true);

	pRectFast->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pRectFast->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	pRectFast->MeshRender()->GetDynamicMaterial(0);
	//pRectFast->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Base_e.dds"));
	pRectFast->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Base_e_Puncture.dds"));
	pRectFast->MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Round.dds"));

	pRectFast->Collider2D()->SetAbsolute(false);
	pRectFast->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	pRectFast->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	pRectFast->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);


	SpawnGameObject(pRectFast, Vec3(-600.f, 0.f, 500.f), 0);



	// Ray Test Object1
	CGameObject* RayTestObj1 = new CGameObject;
	RayTestObj1->SetName(L"RayTestObj1");

	RayTestObj1->AddComponent(new CMeshRender);
	RayTestObj1->AddComponent(new CTransform);
	RayTestObj1->AddComponent(new CCollider2D);

	RayTestObj1->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));

	RayTestObj1->MeshRender()->SetUsingMovingVec(true);

	RayTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	RayTestObj1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	RayTestObj1->MeshRender()->GetDynamicMaterial(0);
	//pRectFast->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Base_e.dds"));
	RayTestObj1->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));
	RayTestObj1->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Base_e_Puncture.dds"));
	RayTestObj1->MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Round.dds"));

	RayTestObj1->Collider2D()->SetAbsolute(false);
	RayTestObj1->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	RayTestObj1->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	RayTestObj1->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);


	SpawnGameObject(RayTestObj1, Vec3(-600.f, 0.f, 600.f), 0);



	// Ray Test Object2
	CGameObject* RayTestObj2 = new CGameObject;
	RayTestObj2->SetName(L"RayTestObj2");

	RayTestObj2->AddComponent(new CMeshRender);
	RayTestObj2->AddComponent(new CTransform);
	RayTestObj2->AddComponent(new CCollider2D);

	RayTestObj2->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));

	RayTestObj2->MeshRender()->SetUsingMovingVec(true);

	RayTestObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	RayTestObj2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	RayTestObj2->MeshRender()->GetDynamicMaterial(0);
	//pRectFast->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Base_e.dds"));
	RayTestObj2->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));
	RayTestObj2->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Base_e_Puncture.dds"));
	RayTestObj2->MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Round.dds"));

	RayTestObj2->Collider2D()->SetAbsolute(false);
	RayTestObj2->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	RayTestObj2->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	RayTestObj2->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);


	SpawnGameObject(RayTestObj2, Vec3(-600.f, 0.f, 700.f), 0);



	// Ray Cube Test Object 1
	CGameObject* RayCubeTestObj1 = new CGameObject;
	RayCubeTestObj1->SetName(L"RayCubeTestObj1");

	RayCubeTestObj1->AddComponent(new CMeshRender);
	RayCubeTestObj1->AddComponent(new CTransform);
	RayCubeTestObj1->AddComponent(new CCollider3D);

	RayCubeTestObj1->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	RayCubeTestObj1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	RayCubeTestObj1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"), 0);
	RayCubeTestObj1->MeshRender()->GetDynamicMaterial(0);

	RayCubeTestObj1->Collider3D()->SetAbsolute(false);
	RayCubeTestObj1->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	RayCubeTestObj1->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	RayCubeTestObj1->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);


	SpawnGameObject(RayCubeTestObj1, Vec3(-600.f, -450.f, 700.f), 0);


	// Ray Cube Test Object 2
	CGameObject* RayCubeTestObj2 = new CGameObject;
	RayCubeTestObj2->SetName(L"RayCubeTestObj1");

	RayCubeTestObj2->AddComponent(new CMeshRender);
	RayCubeTestObj2->AddComponent(new CTransform);
	RayCubeTestObj2->AddComponent(new CCollider3D);

	RayCubeTestObj2->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	RayCubeTestObj2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	RayCubeTestObj2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"), 0);
	RayCubeTestObj2->MeshRender()->GetDynamicMaterial(0);

	RayCubeTestObj2->Collider3D()->SetAbsolute(false);
	RayCubeTestObj2->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	RayCubeTestObj2->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	RayCubeTestObj2->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);


	SpawnGameObject(RayCubeTestObj2, Vec3(-600.f, -450.f, 960.f), 0);


	// Ray Cube Test Object 3
	CGameObject* RayCubeTestObj3 = new CGameObject;
	RayCubeTestObj3->SetName(L"RayCubeTestObj1");

	RayCubeTestObj3->AddComponent(new CMeshRender);
	RayCubeTestObj3->AddComponent(new CTransform);
	RayCubeTestObj3->AddComponent(new CCollider3D);

	RayCubeTestObj3->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 200.f));
	RayCubeTestObj3->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	RayCubeTestObj3->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"), 0);
	RayCubeTestObj3->MeshRender()->GetDynamicMaterial(0);

	RayCubeTestObj3->Collider3D()->SetAbsolute(false);
	RayCubeTestObj3->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	RayCubeTestObj3->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	RayCubeTestObj3->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

	SpawnGameObject(RayCubeTestObj3, Vec3(-600.f, -450.f, 1220.f), 0);



	// TestFastForward
	CGameObject* pRectFast2 = new CGameObject;
	pRectFast2->SetName(L"MoveSphere");
	pRectFast2->AddComponent(new CMeshRender);
	pRectFast2->AddComponent(new CTransform);
	pRectFast2->AddComponent(new CPlayerScript);
	pRectFast2->AddComponent(new CPathFinder);
	pRectFast2->Transform()->SetRelativeScale(Vec3(45.f, 45.f, 45.f));
	pRectFast2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pRectFast2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);

	SpawnGameObject(pRectFast2, Vec3(0.f, 0.f, 0.f), 0);





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
