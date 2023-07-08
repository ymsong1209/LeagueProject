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

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object ����
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera �� ����
	pMainCam->Camera()->SetLayerMaskAll(true);	// ��� ���̾� üũ
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer �� ���������� �ʴ´�.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	
	// ���� �߰�
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

	//// LandScape Object
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
		
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Zilean.fbx");
		
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Zilean"); 
		pObj->Animator3D()->Play(L"Zilean-Zilean_AllAnim", true, 0.5f);
		SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
		
		//Ptr<CMeshData> pMeshData1 = nullptr;
		//CGameObject* pObj1 = nullptr;
		//pMeshData1 = CResMgr::GetInst()->LoadFBX(L"fbx\\amumu.fbx");
		//
		//pObj1 = pMeshData1->Instantiate();
		//pObj1->SetName(L"amumu");		
		//pObj1->Animator3D()->Play(L"Amumu-Amumu-Amumu-Amumu_AllAnim", true, 0.5f);
		//SpawnGameObject(pObj1, Vec3(100.f, 0.f, 0.f), 0);
		//�������
		//pObj->Animator3D()->Play(L"Take 001");
		//�������, ���� �ִϸ��̼� 0.5�� ���� �� �������
		//pObj->Animator3D()->Play(L"Take001", true, 0.5f);
		//�ݺ����, �������������� ù���������� blend����
		//pObj->Animator3D()->Play(L"Take001", true, false);
		//�ݺ����, AnimA->Take001�� 0.5�ʵ��� �ִϸ��̼� blend�ϰ� take001���,take001������������->ù������ ���ƿ��鼭 blend���ϱ� 
		//pObj->Animator3D()->Play(L"Take001", true, false, true, 0.5f);
		//�ݺ����, AnimA->Take001�� 0.5�ʵ��� �ִϸ��̼� blend�ϰ� take001���,take001������������->ù������ ���ƿ��鼭 blend 0.5�ʵ��� �ϱ�
		//pObj->Animator3D()->Play(L"Take001", true, true, true, 0.5f);
		//��� ����
		/*
		IdleAnimation �ݺ���� blend����
		pobj->animator3d()->play(L"IdleAnimation",true,false);
		���ݹ�ư ������ ���� �ִϸ��̼� �������, blending 0.2��
		if(key_pressed(key::lbtn){
		changestate(attack);
		pobj->animator3d()->play(L"AttackAnimation", true,0.2f);
		}
		���� �ִ� ������ �ٽ� idle�� ��ȯ
		if(animator3d()->getcuranim()->isfinish()){
		changestate(idle)
		//Attack->Idle 0.2�� blend�ְ�, idle animation ���, idle anim�� ������������->ù������ �̵��� blend���� 
		pobj->animator3d()->play(L"IdleAnimation",true,false,true,0.2f); 
		}
		
		*/


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

	// �浹 ��ų ���̾� ¦ ����
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");	
}
