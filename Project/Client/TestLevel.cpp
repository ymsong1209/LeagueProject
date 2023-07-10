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
	CGameObject* pLandScape = new CGameObject;
	pLandScape->SetName(L"LandScape");

	pLandScape->AddComponent(new CTransform);
	pLandScape->AddComponent(new CLandScape);

	pLandScape->Transform()->SetRelativeScale(Vec3(200.f, 1000.f, 200.f));

	pLandScape->LandScape()->SetFace(32, 32);
	pLandScape->LandScape()->SetFrustumCheck(false);

	SpawnGameObject(pLandScape, Vec3(0.f, 0.f, 0.f), 0);


	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\BlitzCrank.fbx");
		
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Blitz"); 
		pObj->Animator3D()->LoadAnim(L"animation\\BlitzCrank\\Attack1.anim3d");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\BlitzCrank");
		pObj->Animator3D()->Play(L"BlitzCrank\\Attack1", true, 0.5f);
		pObj->Animator3D()->SetRepeat(true);
		SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
		
		//Ptr<CMeshData> pMeshData1 = nullptr;
		//CGameObject* pObj1 = nullptr;
		//pMeshData1 = CResMgr::GetInst()->LoadFBX(L"fbx\\amumu.fbx");
		//
		//pObj1 = pMeshData1->Instantiate();
		//pObj1->SetName(L"amumu");		
		//pObj1->Animator3D()->Play(L"Amumu-Amumu-Amumu-Amumu_AllAnim", true, 0.5f);
		//SpawnGameObject(pObj1, Vec3(100.f, 0.f, 0.f), 0);
 
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





	// TestFastForward
	CGameObject* pRectFast2 = new CGameObject;
	pRectFast2->SetName(L"ReftFast2");

	pRectFast2->AddComponent(new CMeshRender);
	pRectFast2->AddComponent(new CTransform);

	pRectFast2->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));
	pRectFast2->MeshRender()->SetUsingMovingVec(true);

	pRectFast2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pRectFast2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
	pRectFast2->MeshRender()->GetDynamicMaterial(0);

	pRectFast2->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));
	pRectFast2->MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Morde_Test\\Dots.jpg"));

	SpawnGameObject(pRectFast2, Vec3(-300.f, 0.f, 500.f), 0);


	// Anim Test Object
	CGameObject* pAnimTestObj = new CGameObject;
	pAnimTestObj->SetName(L"AnimTestObj");

	pAnimTestObj->AddComponent(new CMeshRender);
	pAnimTestObj->AddComponent(new CAnimator2D);
	pAnimTestObj->AddComponent(new CTransform);

	pAnimTestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pAnimTestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);

	pAnimTestObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));

	// 테스트를 해보고 싶은데 FindRes 에 CAnim2D가 없어서 못해봤습니다..
	// 추가할라고 마음 먹으면 할 수 있을 것 같긴한데, 너무 크게 바꾸는 것 같아 해보진 않았습니다.
	//pAnimTestObj->Animator2D()->SetCurAnim(CResMgr::GetInst()->FindRes<CAnim2D>(L"Link_Basic").Get());


	SpawnGameObject(pAnimTestObj, Vec3(-200.f, 0.f, 300.f), 0.f);



	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");	
}
