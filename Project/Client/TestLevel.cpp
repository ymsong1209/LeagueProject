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
#include <Engine\CAnimator3D.h>
#include <Engine\CAnim3D.h>


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

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jinx55.fbx");
		//pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(L"meshdata\\jinx55.mdat");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"jinx");
		pObj->AddComponent(new CPlayerScript);
		pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-90.f), 0.f, 0.f));

		pObj->Animator3D()->CreateClipToAnimT((double)0, (double)1.43, L"animation\\jinx55_Clip.anim", L"animation\\jinx.anim");
		//���� �� �޽������� Instantiate()�Լ��� ������ �˰����� ���ο��� �ε��ϴ� FBX �̸� (ex. jinx55) �� ���� L"animation\\jinx55_Clip.anim" ��� �ִ�Ŭ�� �ִϸ��̼��� ��������ϴ�.
		//�׷��� �ű⼭ �������� �߶� ���ο� �ִϸ��̼��� ����� �ʹٸ� ���Ͱ��� ���� Ŭ���̸���, ���θ��� �ִϸ��̼� �̸��� ������  CreateClipToAnimT()�� , CreateClipToAnimF �Լ� ���ڿ� �־��ָ� �˴ϴ�.
		pObj->Animator3D()->Play(L"animation\\jinx.anim", true, true, true);

		SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), 0);
	}



	// �浹 ��ų ���̾� ¦ ����
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");	
}
