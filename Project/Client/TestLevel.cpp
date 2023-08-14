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
#include <Script\CSoundTestScript.h>
#include <Engine\CPathFindMgr.h>
#include <Script\CCharacterUIScript.h>

#include "CLevelSaveLoad.h"
#include <Script/CScriptMgr.h>
#include <Script/CWorldHPSpawnScript.h>
#include <Script\CWorldHPUIScript.h>
#include <Script\CInventoryUIScript.h>
#include <Script/CJinxScript.h>
#include <Script\CMinimapUIScript.h>
#include <Script\CMouseCursorUIScript.h>
#include <Script/CTurretScript.h>
#include <Script\CFontUIScript.h>
#include <Script\CScorePanelScript.h>
#include <Script/CJinxScript.h>
#include <Script/CAttackRangeScript.h>


#include <Script/CGrompScript.h>
#include <Script/CMurkWolfScript.h>
#include <Script/CMurkWolfMiniScript.h>
#include <Script/CRazorBeakScript.h>
#include <Script/CRazorBeakMiniScript.h>
#include <Script/CKrugScript.h>
#include <Script/CKrugMiniScript.h>
#include <Script/CBlueScript.h>
#include <Script/CRedScript.h>
#include <Script\CMinionHPRatioScript.h>
#include <Script\CDragonHPUIScript.h>
#include <Script\CJungleMobHPScript.h>
#include <Script\CJungleMINIHPScript.h>
#include <Script/CInGameCameraScript.h>
#include <Script/CDragonScript.h>
#include <Script/CShopKeeperNorthScript.h>
#include <Script/CShopKeeperSouthScript.h>

#include <Script/CBushScript.h>


void CreateTestLevel()
{
	// 귀신 퇴치용 주석
	// 
	//return;	
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);
	//pCurLevel->ChangeState(LEVEL_STATE::STOP);
	CTimeMgr::GetInst()->SetTimeScale(1.f);    // TestLevel에서 강제로 PLAY모드로 전환해서 DT를 흐르게 하기 위함. 추후 삭제

	//롤맵 레이어에는 롤맵만 넣을것!
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Lolmap");   // land
	pCurLevel->GetLayer(2)->SetName(L"Map_Wall");   // map_wall (맵 벽)
	pCurLevel->GetLayer(3)->SetName(L"Structure");   //Nexus, Turret, Inhibitor
	pCurLevel->GetLayer(4)->SetName(L"Mob");   // Minion, JungleMob
	pCurLevel->GetLayer(5)->SetName(L"Player");   // Champion
	pCurLevel->GetLayer(6)->SetName(L"AttackRange");   //  공격범위
	pCurLevel->GetLayer(7)->SetName(L"SkillProjectile");   // Skill 
	pCurLevel->GetLayer(8)->SetName(L"Effect");   // Effect
	pCurLevel->GetLayer(9)->SetName(L"Bush");   // Bush
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");   // UI

	CCollisionMgr::GetInst()->LayerCheck(L"Mob", L"Mob");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Player");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"SkillProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Mob", L"SkillProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Structure", L"SkillProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Structure");
	CCollisionMgr::GetInst()->LayerCheck(L"AttackRange", L"Player");
	CCollisionMgr::GetInst()->LayerCheck(L"AttackRange", L"Mob");
	CCollisionMgr::GetInst()->LayerCheck(L"AttackRange", L"Structure");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Map_Wall");
	CCollisionMgr::GetInst()->LayerCheck(L"Mob", L"Map_Wall");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Bush");

	//Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");
	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	//pMainCam->AddComponent(new CCameraMoveScript);
	pMainCam->AddComponent(new CInGameCameraScript);
	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.
	pMainCam->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(60.f), 0.f, 0.f));

	SpawnGameObject(pMainCam, Vec3(120.f, 525.f, -152.f), 0);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");
	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);
	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);
	pUICam->Camera()->SetLayerMask(31, true);	// 모든 레이어 체크
	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


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
	pLightObj->Light3D()->SetLightDiffuse(Vec3(0.4f, 0.4f, 0.4f));
	pLightObj->Light3D()->SetLightSpecular(Vec3(0.f, 0.f, 0.f));
	pLightObj->Light3D()->SetLightAmbient(Vec3(0.4f, 0.4f, 0.4f));


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
	LoLMapCollider->Collider2D()->SetDrawCollision(false);
	LoLMapCollider->Transform()->SetGizmoObjExcept(true);
	SpawnGameObject(LoLMapCollider, Vec3(0.f, 0.f, 0.f), L"Lolmap");


	CGameObject* ShopKeeperSouth = new CGameObject;
	ShopKeeperSouth->AddComponent(new CTransform);
	ShopKeeperSouth->AddComponent(new CAnimator3D);
	ShopKeeperSouth->AddComponent(new CMeshRender);
	ShopKeeperSouth->AddComponent(new CShopKeeperSouthScript);
	ShopKeeperSouth->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	ShopKeeperSouth->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-87.f), 0.f));
	ShopKeeperSouth->SetName(L"ShopKeeperSouth");
	SpawnGameObject(ShopKeeperSouth, Vec3(37.f, 30.f, 173.f), L"Default");

	CGameObject* ShopKeeperNorth = new CGameObject;
	ShopKeeperNorth->AddComponent(new CTransform);
	ShopKeeperNorth->AddComponent(new CAnimator3D);
	ShopKeeperNorth->AddComponent(new CMeshRender);
	ShopKeeperNorth->AddComponent(new CShopKeeperNorthScript);
	ShopKeeperNorth->Transform()->SetRelativeScale(Vec3(0.3f, 0.3f, 0.3f));
	ShopKeeperNorth->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-39.f), 0.f));
	ShopKeeperNorth->SetName(L"ShopKeeperNorth");
	SpawnGameObject(ShopKeeperNorth, Vec3(2021, 30.f, 2154.f), L"Default");

	

	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\LoLMapRot19Size30.fbx");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"LoLMapRot19Size30");
		//pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->AddComponent(new CCollider2D);
		//pObj->Collider2D()->SetAbsolute(true);
		//pObj->Collider2D()->SetDrawCollision(false);
		//pObj->GetRenderComponent()->SetRaySightCulling(false);

		//pMeshData = nullptr;
		//pObj = nullptr;
		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"jungle_blue");
		//pObj->GetRenderComponent()->SetFrustumCheck(false);
		//pObj->Animator3D()->Play(L"jungle_blue-jungle_blue_AllAnim", true, 0.5f);
		//pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));
		//SpawnGameObject(pObj, Vec3(190.f, 0.f, 607.f), 0);

		/*pMeshData = nullptr;
		pObj = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Malphite.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Malphite");
		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->AddComponent(new CPlayerScript);
		pObj->AddComponent(new CJinxScript);
		pObj->AddComponent(new CPathFinder);
		pObj->AddComponent(new CCollider3D);
		pObj->AddComponent(new CCollider2D);
		pObj->AddComponent(new CFsm);
		
		pObj->Collider2D()->SetAbsolute(false);
		pObj->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		pObj->Collider2D()->SetOffsetScale(Vec2(20.f, 20.f));
		pObj->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		
		pObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
		pObj->Collider3D()->SetAbsolute(true);
		pObj->Collider3D()->SetOffsetScale(Vec3(30.f, 30.f, 30.f));
		pObj->Collider3D()->SetDrawCollision(false);
		pObj->Animator3D()->PlayRepeat(L"Jinx\\Idle1_Base", true,true,0.1f);
		pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
		
		pObj->Transform()->SetUseMouseOutline(true);
		CJinxScript* jinxscript = pObj->GetScript<CJinxScript>();
		jinxscript->SetFaction(Faction::BLUE);
		
		SpawnGameObject(pObj, Vec3(0, 0, 0), L"Player");

		CGameObject* JinxAttackRange = new CGameObject;
		JinxAttackRange->SetName(L"AttackRange");
		JinxAttackRange->AddComponent(new CTransform);
		JinxAttackRange->AddComponent(new CCollider2D);
		JinxAttackRange->AddComponent(new CAttackRangeScript);
		JinxAttackRange->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		JinxAttackRange->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		JinxAttackRange->Collider2D()->SetAbsolute(true);
		JinxAttackRange->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
		pObj->AddChild(JinxAttackRange);
		JinxAttackRange->ChangeLayer(L"AttackRange");*/



	}


	PlaceLand();
	//SpawnJungleMob();
	//PlaceStructure();

	CGameObject* pBushObj = new CGameObject;
	pBushObj->SetName(L"TestBush");

	pBushObj->AddComponent(new CTransform);
	pBushObj->AddComponent(new CCollider3D);
	pBushObj->AddComponent(new CBushScript);

	pBushObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 100.f));
	pBushObj->Transform()->SetRelativeRot(Vec3(0.f, -13.f, 0.f));
	pBushObj->Transform()->SetRelativePos(Vec3(701.f, 0.f, 469.f));

	pBushObj->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	pBushObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);

	SpawnGameObject(pBushObj, Vec3(701.f, 0.f, 469.f), L"Bush");





	/*CGameObject* pSoundTestObj = new CGameObject;
	pSoundTestObj->SetName(L"pSoundTestObj");

	pSoundTestObj->AddComponent(new CMeshRender);
	pSoundTestObj->AddComponent(new CTransform);
	pSoundTestObj->AddComponent(new CSoundTestScript);

	pSoundTestObj->Transform()->SetRelativeScale(Vec3(45.f, 45.f, 45.f));
	pSoundTestObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pSoundTestObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3D_DeferredMtrl"), 0);

	SpawnGameObject(pSoundTestObj, Vec3(0.f, 0.f, 0.f), 0.f);*/
	// 
	// 충돌 시킬 레이어 짝 지정
	//CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
	//CCollisionMgr::GetInst()->LayerCheck(L"Default", L"Default");
}

void PlaceLand()
{
	//LoLMap 로딩
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		// 파일 경로 만들기
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += L"lolmap.txt";

		// 읽기모드로 파일열기
		std::wifstream file(strFilePath);
		std::wstring line;

		while (std::getline(file, line))
		{
			std::wistringstream iss(line);
			std::wstring landName;
			std::wstring ignore;
			float scale;
			Vec3 offset;

			if (!(iss >> landName >> ignore >> scale >> ignore >> offset.x >> offset.y >> offset.z)) { break; }

			wstring FBXFilePath = L"fbx\\" + landName + L".fbx";
			wstring FBXFileName = landName + L".fbx";
			pMeshData = CResMgr::GetInst()->LoadFBX(FBXFilePath);
			pObj = pMeshData->Instantiate();
			pObj->SetName(FBXFileName);

			pObj->GetRenderComponent()->SetFrustumCheck(true);
			pObj->GetRenderComponent()->SetShowDebugBound(false);
			pObj->Transform()->SetGizmoObjExcept(false);

			pObj->GetRenderComponent()->SetBounding(scale);
			pObj->GetRenderComponent()->SetBoundingBoxOffsetUse(true);
			pObj->GetRenderComponent()->SetBoundingBoxOffset(offset);

			SpawnGameObject(pObj, Vec3(0.f, 0.f, 0.f), L"Lolmap");
		}
	}
}

void PlaceStructure()
{
	Ptr<CMeshData> pMeshData = nullptr;
	CGameObject* pObj = nullptr;
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
	pObj->Animator3D()->PlayRepeat(L"nexus\\sruap_order_idle.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\sruap_ordernexus_tx_cm_clear.png"));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(229.7f, 15.9f, 241.5f), L"Structure");

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
	pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-45.48f), 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(537.71f, 14.2f, 546.9f), L"Structure");

	pMeshData = nullptr;
	pObj = nullptr;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"blue_Inhibitor2");
	pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
	pObj->GetRenderComponent()->SetFrustumCheck(false);
	pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-89.48f), 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(169.86f, 14.2f, 527.02f), L"Structure");

	pMeshData = nullptr;
	pObj = nullptr;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"blue_Inhibitor2");
	pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
	pObj->GetRenderComponent()->SetFrustumCheck(false);
	pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(2.f), 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(634.97f, 14.2f, 183.08f), L"Structure");

	//----------RED-------------------------------

	pMeshData = nullptr;
	pObj = nullptr;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\nexus.fbx");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"red_nexus");
	pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\nexus");
	pObj->GetRenderComponent()->SetFrustumCheck(false);
	pObj->Animator3D()->PlayRepeat(L"nexus\\sruap_order_idle.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\nexus_Mat_Red.mtrl"), 1);
	pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(1952.174f, 15.26f, 1956.22f), L"Structure");

	pMeshData = nullptr;
	pObj = nullptr;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"red_Inhibitor1");
	pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
	pObj->GetRenderComponent()->SetFrustumCheck(false);
	pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
	pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(0.f), XMConvertToRadians(-180.f)));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(1661.7f, 14.8f, 2013.9f), L"Structure");

	pMeshData = nullptr;
	pObj = nullptr;
	pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Inhibitor.fbx");
	pObj = pMeshData->Instantiate();
	pObj->SetName(L"red_Inhibitor2");
	pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Inhibitor");
	pObj->GetRenderComponent()->SetFrustumCheck(false);
	pObj->Animator3D()->PlayRepeat(L"Inhibitor\\inhibitor_idle1.anm_skinned_mesh.001", true, true, 0.1f);
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\inhibitor_red_Mtrl.mtrl"), 0);
	pObj->MeshRender()->GetMaterial(1)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\alphaTex.png"));
	pObj->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(45.f), XMConvertToRadians(-180.f)));
	pObj->Transform()->SetRelativeScale(Vec3(0.18f, 0.18f, 0.18f));
	SpawnGameObject(pObj, Vec3(1711.f, 14.8f, 1721.f), L"Structure");


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
	SpawnGameObject(pObj, Vec3(2006.9f, 14.8f, 1670.1f), L"Structure");
}

void SpawnJungleMob()
{
	//블루팀
	{
		//블루팀 두꺼비
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\gromp.fbx");
			CGameObject* Gromp = nullptr;
			Gromp = pMeshData->Instantiate();
			Gromp->AddComponent(new CGrompScript);
			Gromp->SetName(L"SOUTH_Gromp");
			Gromp->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			Gromp->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(270.f), 0.f));
			CGrompScript* Script = Gromp->GetScript<CGrompScript>();
			Script->SetAggroPos(Vec3(323.f, 0.f, 1242.f));
			Script->SetUnitType(UnitType::SOUTH_GROMP);
			SpawnGameObject(Gromp, Vec3(323.f, 10.f, 1242.f), L"Mob");

			CGameObject* GrompAggro = new CGameObject;
			GrompAggro->SetName(L"SOUTH_GrompAggroRadius");
			GrompAggro->AddComponent(new CTransform);
			GrompAggro->AddComponent(new CCollider2D);
			GrompAggro->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 150.f));
			GrompAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			GrompAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			GrompAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(GrompAggro, Vec3(323.f, 0.f, 1242.f), 0);

			CGameObject* GrompHP = new CGameObject;
			GrompHP->SetName(L"GrompHP");
			GrompHP->AddComponent(new CTransform);
			GrompHP->AddComponent(new CMeshRender);
			GrompHP->AddComponent(new CJungleMobHPScript);
			Gromp->AddChild(GrompHP);
		}

		//블루팀 늑대
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf.fbx");
			CGameObject* MurkWolf = nullptr;
			MurkWolf = pMeshData->Instantiate();
			MurkWolf->AddComponent(new CMurkWolfScript);
			MurkWolf->SetName(L"SOUTH_MurkWolf");
			MurkWolf->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			MurkWolf->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(24.f), 0.f));
			CMurkWolfScript* script = MurkWolf->GetScript <CMurkWolfScript>();
			script->SetAggroPos(Vec3(564.f, 0.f, 959.f));
			script->SetUnitType(UnitType::SOUTH_MURKWOLF);
			SpawnGameObject(MurkWolf, Vec3(550.f, 15.f, 944.f), L"Mob");

			CGameObject* MurkWolfAggro = new CGameObject;
			MurkWolfAggro->SetName(L"SOUTH_MurkWolfAggroRadius");
			MurkWolfAggro->AddComponent(new CTransform);
			MurkWolfAggro->AddComponent(new CCollider2D);
			MurkWolfAggro->Transform()->SetRelativeScale(Vec3(190, 190.f, 190.f));
			MurkWolfAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			MurkWolfAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			MurkWolfAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(MurkWolfAggro, Vec3(564.f, 0.f, 959.f), 0);

			CGameObject* MurkWolfHP = new CGameObject;
			MurkWolfHP->SetName(L"SOUTH_MurkWolfHP");
			MurkWolfHP->AddComponent(new CTransform);
			MurkWolfHP->AddComponent(new CMeshRender);
			MurkWolfHP->AddComponent(new CJungleMobHPScript);
			MurkWolf->AddChild(MurkWolfHP);
		}
		//블루팀 늑대 째깐이(좌)
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			CGameObject* MurkWolf_Mini = nullptr;
			MurkWolf_Mini = pMeshData->Instantiate();
			MurkWolf_Mini->AddComponent(new CMurkWolfMiniScript);
			MurkWolf_Mini->SetName(L"SOUTH_MurkWolf_Mini_L");
			MurkWolf_Mini->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15);
			MurkWolf_Mini->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(36.f), 0.f));
			CMurkWolfMiniScript* Script = MurkWolf_Mini->GetScript<CMurkWolfMiniScript>();
			Script->SetAggroPos(Vec3(564.f, 0.f, 964.f));
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF_MINI_L);
			SpawnGameObject(MurkWolf_Mini, Vec3(552.f, 15.f, 964.f), L"Mob");

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"SOUTH_MurkWolf_Mini_L_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			MurkWolf_Mini->AddChild(MurkWolf_MiniHP);
		}
		//블루팀 늑대 째깐이(우)
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			CGameObject* MurkWolf_Mini = nullptr;
			MurkWolf_Mini = pMeshData->Instantiate();
			MurkWolf_Mini->AddComponent(new CMurkWolfMiniScript);
			MurkWolf_Mini->SetName(L"SOUTH_MurkWolf_Mini_R");
			MurkWolf_Mini->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15f);
			MurkWolf_Mini->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(36.f), 0.f));
			CMurkWolfMiniScript* Script = MurkWolf_Mini->GetScript<CMurkWolfMiniScript>();
			Script->SetAggroPos(Vec3(564.f, 0.f, 964.f));
			Script->SetUnitType(UnitType::SOUTH_MURKWOLF_MINI_R);
			SpawnGameObject(MurkWolf_Mini, Vec3(580.f, 15.f, 944.f), L"Mob");

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"SOUTH_MurkWolf_Mini_R_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			MurkWolf_Mini->AddChild(MurkWolf_MiniHP);
		}
		//블루팀 돌거북
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug.fbx");
			CGameObject* Krug = nullptr;
			Krug = pMeshData->Instantiate();
			Krug->AddComponent(new CKrugScript);
			Krug->SetName(L"SOUTH_Krug");
			Krug->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			Krug->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			CKrugScript* Script = Krug->GetScript<CKrugScript>();
			Script->SetAggroPos(Vec3(1238.f, 0.f, 389.f));
			Script->SetUnitType(UnitType::SOUTH_KRUG);
			SpawnGameObject(Krug, Vec3(1221.f, 15.f, 379.f), L"Mob");

			CGameObject* KrugAggro = new CGameObject;
			KrugAggro->SetName(L"SOUTH_KrugAggroRadius");
			KrugAggro->AddComponent(new CTransform);
			KrugAggro->AddComponent(new CCollider2D);
			KrugAggro->Transform()->SetRelativeScale(Vec3(170, 170.f, 170.f));
			KrugAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			KrugAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			KrugAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(KrugAggro, Vec3(1238.f, 0.f, 389.f), 0);

			CGameObject* KrugHP = new CGameObject;
			KrugHP->SetName(L"SOUTH_KrugHP");
			KrugHP->AddComponent(new CTransform);
			KrugHP->AddComponent(new CMeshRender);
			KrugHP->AddComponent(new CJungleMobHPScript);
			Krug->AddChild(KrugHP);

		}
		//블루팀 돌거북 째깐이
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug_Mini.fbx");
			CGameObject* Krug_Mini = nullptr;
			Krug_Mini = pMeshData->Instantiate();
			Krug_Mini->AddComponent(new CKrugMiniScript);
			Krug_Mini->SetName(L"SOUTH_Krug_Mini");
			Krug_Mini->Transform()->SetRelativeScale(0.12f, 0.12f, 0.12f);
			Krug_Mini->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			CKrugMiniScript* Script = Krug_Mini->GetScript<CKrugMiniScript>();
			Script->SetAggroPos(Vec3(1238.f, 0.f, 389.f));
			Script->SetUnitType(UnitType::SOUTH_KRUG_MINI);
			SpawnGameObject(Krug_Mini, Vec3(1252.f, 15.f, 379.f), L"Mob");

			CGameObject* Krug_MiniHP = new CGameObject;
			Krug_MiniHP->SetName(L"SOUTH_Krug_Mini_HP");
			Krug_MiniHP->AddComponent(new CTransform);
			Krug_MiniHP->AddComponent(new CMeshRender);
			Krug_MiniHP->AddComponent(new CJungleMINIHPScript);
			Krug_Mini->AddChild(Krug_MiniHP);
		}
		//블루팀 칼날부리
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak.fbx");
			CGameObject* RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakScript);
			RazorBeak->SetName(L"SOUTH_RazorBeak");
			RazorBeak->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-66.f), 0.f));
			CRazorBeakScript* Script = RazorBeak->GetScript<CRazorBeakScript>();
			Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK);
			SpawnGameObject(RazorBeak, Vec3(1008.f, 15.f, 800.f), L"Mob");

			CGameObject* RazorBeakAggro = new CGameObject;
			RazorBeakAggro->SetName(L"SOUTH_RazorBeakAggroRadius");
			RazorBeakAggro->AddComponent(new CTransform);
			RazorBeakAggro->AddComponent(new CCollider2D);
			RazorBeakAggro->Transform()->SetRelativeScale(Vec3(170, 170.f, 170.f));
			RazorBeakAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			RazorBeakAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			RazorBeakAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(RazorBeakAggro, Vec3(1033.f, 0.f, 782.f), 0);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeakHP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMobHPScript);
			RazorBeak->AddChild(RazorBeakHP);
		}

		//블루팀 칼날부리 째깐이
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			CGameObject* RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"SOUTH_RazorBeak_Mini1");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			CRazorBeakMiniScript* Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_1);
			SpawnGameObject(RazorBeak, Vec3(1026.f, 15.f, 810.f), L"Mob");

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini1_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"SOUTH_RazorBeak_Mini2");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(73.f), 0.f));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_2);
			SpawnGameObject(RazorBeak, Vec3(1048.f, 15.f, 799.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini2_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"SOUTH_RazorBeak_Mini3");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-90.f), 0.f));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_3);
			SpawnGameObject(RazorBeak, Vec3(1031.f, 15.f, 782.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini3_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"SOUTH_RazorBeak_Mini4");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(4.f), 0.f));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_4);
			SpawnGameObject(RazorBeak, Vec3(1012.f, 15.f, 765.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini4_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"SOUTH_RazorBeak_Mini5");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(69.f), 0.f));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1033.f, 0.f, 782.f));
			Script->SetUnitType(UnitType::SOUTH_RAZORBEAK_MINI_5);
			SpawnGameObject(RazorBeak, Vec3(1048.f, 15.f, 767.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"SOUTH_RazorBeak_Mini5_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);
		}
		//블루팀 블루
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
			CGameObject* Blue = nullptr;
			Blue = pMeshData->Instantiate();
			Blue->AddComponent(new CBlueScript);
			Blue->SetName(L"South_Blue");
			Blue->Transform()->SetRelativeScale(0.2f, 0.2f, 0.2f);
			Blue->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			CBlueScript* Script = Blue->GetScript<CBlueScript>();
			Script->SetAggroPos(Vec3(563.f, 0.f, 1164.f));
			Script->SetUnitType(UnitType::SOUTH_BLUE);
			SpawnGameObject(Blue, Vec3(563.f, 15.f, 1164.f), L"Mob");

			CGameObject* BlueAggro = new CGameObject;
			BlueAggro->SetName(L"SOUTH_BlueAggroRadius");
			BlueAggro->AddComponent(new CTransform);
			BlueAggro->AddComponent(new CCollider2D);
			BlueAggro->Transform()->SetRelativeScale(Vec3(170, 170.f, 170.f));
			BlueAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			BlueAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			BlueAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(BlueAggro, Vec3(563.f, 0.f, 1164.f), 0);

			CGameObject* BlueHP = new CGameObject;
			BlueHP->SetName(L"South_BlueHP");
			BlueHP->AddComponent(new CTransform);
			BlueHP->AddComponent(new CMeshRender);
			BlueHP->AddComponent(new CJungleMobHPScript);
			Blue->AddChild(BlueHP);
		}
		//블루팀 레드
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_red.fbx");
			CGameObject* Red = nullptr;
			Red = pMeshData->Instantiate();
			Red->AddComponent(new CRedScript);
			Red->SetName(L"South_Red");
			Red->Transform()->SetRelativeScale(0.35f, 0.35f, 0.35f);
			Red->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(46.f), 0.f));
			CRedScript* Script = Red->GetScript<CRedScript>();
			Script->SetAggroPos(Vec3(1123, 15.f, 559.f));
			Script->SetUnitType(UnitType::SOUTH_RED);
			SpawnGameObject(Red, Vec3(1136, 15.f, 580.f), L"Mob");

			CGameObject* RedAggro = new CGameObject;
			RedAggro->SetName(L"SOUTH_RedAggroRadius");
			RedAggro->AddComponent(new CTransform);
			RedAggro->AddComponent(new CCollider2D);
			RedAggro->Transform()->SetRelativeScale(Vec3(190, 190.f, 190.f));
			RedAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			RedAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			RedAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(RedAggro, Vec3(1123, 15.f, 559.f), 0);

			CGameObject* RedHP = new CGameObject;
			RedHP->SetName(L"South_RedHP");
			RedHP->AddComponent(new CTransform);
			RedHP->AddComponent(new CMeshRender);
			RedHP->AddComponent(new CJungleMobHPScript);
			Red->AddChild(RedHP);
		}
	}
	//--------------------------------------------

	//-------------------------------------------

	//레드팀
	{
		//레드팀 두꺼비
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\gromp.fbx");
			CGameObject* Gromp = nullptr;
			Gromp = pMeshData->Instantiate();
			Gromp->AddComponent(new CGrompScript);
			Gromp->SetName(L"NORTH_Gromp");
			Gromp->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			Gromp->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(58.f), 0.f));
			CGrompScript* Script = Gromp->GetScript<CGrompScript>();
			Script->SetAggroPos(Vec3(1823.f, 0.f, 943.f));
			Script->SetUnitType(UnitType::NORTH_GROMP);
			SpawnGameObject(Gromp, Vec3(1859.f, 10.f, 956.f), L"Mob");

			CGameObject* GrompAggro = new CGameObject;
			GrompAggro->SetName(L"NORTH_GrompAggroRadius");
			GrompAggro->AddComponent(new CTransform);
			GrompAggro->AddComponent(new CCollider2D);
			GrompAggro->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 150.f));
			GrompAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			GrompAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			GrompAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(GrompAggro, Vec3(1823.f, 0.f, 943.f), 0);
		}

		//레드팀 늑대
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf.fbx");
			CGameObject* MurkWolf = nullptr;
			MurkWolf = pMeshData->Instantiate();
			MurkWolf->AddComponent(new CMurkWolfScript);
			MurkWolf->SetName(L"NORTH_MurkWolf");
			MurkWolf->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			MurkWolf->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(-21.f), XMConvertToRadians(-180.f)));
			CMurkWolfScript* script = MurkWolf->GetScript <CMurkWolfScript>();
			script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
			script->SetUnitType(UnitType::NORTH_MURKWOLF);
			SpawnGameObject(MurkWolf, Vec3(1619.f, 15.f, 1236.f), L"Mob");

			CGameObject* MurkWolfAggro = new CGameObject;
			MurkWolfAggro->SetName(L"NORTH_MurkWolfAggroRadius");
			MurkWolfAggro->AddComponent(new CTransform);
			MurkWolfAggro->AddComponent(new CCollider2D);
			MurkWolfAggro->Transform()->SetRelativeScale(Vec3(190, 190.f, 190.f));
			MurkWolfAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			MurkWolfAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			MurkWolfAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(MurkWolfAggro, Vec3(1633.f, 0.f, 1265.f), 0);

			CGameObject* MurkWolfHP = new CGameObject;
			MurkWolfHP->SetName(L"NORTH_MurkWolfHP");
			MurkWolfHP->AddComponent(new CTransform);
			MurkWolfHP->AddComponent(new CMeshRender);
			MurkWolfHP->AddComponent(new CJungleMobHPScript);
			MurkWolf->AddChild(MurkWolfHP);
		}
		//레드팀 늑대 째깐이(좌)
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			CGameObject* MurkWolf_Mini = nullptr;
			MurkWolf_Mini = pMeshData->Instantiate();
			MurkWolf_Mini->AddComponent(new CMurkWolfMiniScript);
			MurkWolf_Mini->SetName(L"NORTH_MurkWolf_Mini_L");
			MurkWolf_Mini->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15);
			MurkWolf_Mini->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(-30.f), XMConvertToRadians(-180.f)));
			CMurkWolfMiniScript* Script = MurkWolf_Mini->GetScript<CMurkWolfMiniScript>();
			Script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
			Script->SetUnitType(UnitType::NORTH_MURKWOLF_MINI_L);
			SpawnGameObject(MurkWolf_Mini, Vec3(1592.f, 19.f, 1232.f), L"Mob");

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"NORTH_MurkWolf_Mini_L_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			MurkWolf_Mini->AddChild(MurkWolf_MiniHP);
		}
		//레드팀 늑대 째깐이(우)
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\MurkWolf_Mini.fbx");
			CGameObject* MurkWolf_Mini = nullptr;
			MurkWolf_Mini = pMeshData->Instantiate();
			MurkWolf_Mini->AddComponent(new CMurkWolfMiniScript);
			MurkWolf_Mini->SetName(L"NORTH_MurkWolf_Mini_R");
			MurkWolf_Mini->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15f);
			MurkWolf_Mini->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(-180.f), XMConvertToRadians(-30.f), XMConvertToRadians(-180.f)));
			CMurkWolfMiniScript* Script = MurkWolf_Mini->GetScript<CMurkWolfMiniScript>();
			Script->SetAggroPos(Vec3(1633.f, 0.f, 1265.f));
			Script->SetUnitType(UnitType::NORTH_MURKWOLF_MINI_R);
			SpawnGameObject(MurkWolf_Mini, Vec3(1634, 15.f, 1208.f), L"Mob");

			CGameObject* MurkWolf_MiniHP = new CGameObject;
			MurkWolf_MiniHP->SetName(L"NORTH_MurkWolf_Mini_R_HP");
			MurkWolf_MiniHP->AddComponent(new CTransform);
			MurkWolf_MiniHP->AddComponent(new CMeshRender);
			MurkWolf_MiniHP->AddComponent(new CJungleMINIHPScript);
			MurkWolf_Mini->AddChild(MurkWolf_MiniHP);
		}
		//레드팀 돌거북
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug.fbx");
			CGameObject* Krug = nullptr;
			Krug = pMeshData->Instantiate();
			Krug->AddComponent(new CKrugScript);
			Krug->SetName(L"NORTH_Krug");
			Krug->Transform()->SetRelativeScale(0.25f, 0.25f, 0.25f);
			Krug->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-4.f), XMConvertToRadians(-180.f)));
			CKrugScript* Script = Krug->GetScript<CKrugScript>();
			Script->SetAggroPos(Vec3(942.f, 0.f, 1845.f));
			Script->SetUnitType(UnitType::NORTH_KRUG);
			SpawnGameObject(Krug, Vec3(966, 15.f, 1800.f), L"Mob");

			CGameObject* KrugAggro = new CGameObject;
			KrugAggro->SetName(L"NORTH_KrugAggroRadius");
			KrugAggro->AddComponent(new CTransform);
			KrugAggro->AddComponent(new CCollider2D);
			KrugAggro->Transform()->SetRelativeScale(Vec3(170, 170.f, 170.f));
			KrugAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			KrugAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			KrugAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(KrugAggro, Vec3(942.f, 0.f, 1845.f), 0);

			CGameObject* KrugHP = new CGameObject;
			KrugHP->SetName(L"NORTH_KrugHP");
			KrugHP->AddComponent(new CTransform);
			KrugHP->AddComponent(new CMeshRender);
			KrugHP->AddComponent(new CJungleMobHPScript);
			Krug->AddChild(KrugHP);
		}
		//레드팀 돌거북 째깐이
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Krug_Mini.fbx");
			CGameObject* Krug_Mini = nullptr;
			Krug_Mini = pMeshData->Instantiate();
			Krug_Mini->AddComponent(new CKrugMiniScript);
			Krug_Mini->SetName(L"NORTH_Krug_Mini");
			Krug_Mini->Transform()->SetRelativeScale(0.15f, 0.15f, 0.15f);
			Krug_Mini->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(4.f), XMConvertToRadians(180.f)));
			CKrugMiniScript* Script = Krug_Mini->GetScript<CKrugMiniScript>();
			Script->SetAggroPos(Vec3(942.f, 0.f, 1845.f));
			Script->SetUnitType(UnitType::NORTH_KRUG_MINI);
			SpawnGameObject(Krug_Mini, Vec3(935.f, 15.f, 1791.f), L"Mob");

			CGameObject* Krug_MiniHP = new CGameObject;
			Krug_MiniHP->SetName(L"NORTH_Krug_Mini_HP");
			Krug_MiniHP->AddComponent(new CTransform);
			Krug_MiniHP->AddComponent(new CMeshRender);
			Krug_MiniHP->AddComponent(new CJungleMINIHPScript);
			Krug_Mini->AddChild(Krug_MiniHP);
		}
		//레드팀 칼날부리
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak.fbx");
			CGameObject* RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakScript);
			RazorBeak->SetName(L"NORTH_RazorBeak");
			RazorBeak->Transform()->SetRelativeScale(0.18f, 0.18f, 0.18f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(90.f), XMConvertToRadians(180.f)));
			CRazorBeakScript* Script = RazorBeak->GetScript<CRazorBeakScript>();
			Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK);
			SpawnGameObject(RazorBeak, Vec3(1170.f, 15.f, 1390.f), L"Mob");

			CGameObject* RazorBeakAggro = new CGameObject;
			RazorBeakAggro->SetName(L"NORTH_RazorBeakAggroRadius");
			RazorBeakAggro->AddComponent(new CTransform);
			RazorBeakAggro->AddComponent(new CCollider2D);
			RazorBeakAggro->Transform()->SetRelativeScale(Vec3(170, 170.f, 170.f));
			RazorBeakAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			RazorBeakAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			RazorBeakAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(RazorBeakAggro, Vec3(1145.f, 0.f, 1438.f), 0);

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeakHP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMobHPScript);
			RazorBeak->AddChild(RazorBeakHP);
		}

		//레드팀 칼날부리 째깐이
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\RazorBeak_Mini.fbx");
			CGameObject* RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"NORTH_RazorBeak_Mini1");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-46.f), XMConvertToRadians(180.f)));
			CRazorBeakMiniScript* Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_1);
			SpawnGameObject(RazorBeak, Vec3(1139, 15.f, 1419.f), L"Mob");

			CGameObject* RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini1_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"NORTH_RazorBeak_Mini2");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(52.f), XMConvertToRadians(180.f)));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_2);
			SpawnGameObject(RazorBeak, Vec3(1176, 15.f, 1419.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini2_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"NORTH_RazorBeak_Mini3");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(90.f), 0.f));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_3);
			SpawnGameObject(RazorBeak, Vec3(1159, 15.f, 1405.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini3_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"NORTH_RazorBeak_Mini4");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(0.f), XMConvertToRadians(-90.f), XMConvertToRadians(0.f)));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_4);
			SpawnGameObject(RazorBeak, Vec3(1140, 15.f, 1385.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini4_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);

			RazorBeak = nullptr;
			RazorBeak = pMeshData->Instantiate();
			RazorBeak->AddComponent(new CRazorBeakMiniScript);
			RazorBeak->SetName(L"NORTH_RazorBeak_Mini5");
			RazorBeak->Transform()->SetRelativeScale(0.14f, 0.14f, 0.14f);
			RazorBeak->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(0.f), XMConvertToRadians(180.f)));
			Script = RazorBeak->GetScript<CRazorBeakMiniScript>();
			Script->SetAggroPos(Vec3(1145.f, 0.f, 1438.f));
			Script->SetUnitType(UnitType::NORTH_RAZORBEAK_MINI_5);
			SpawnGameObject(RazorBeak, Vec3(1157, 15.f, 1370.f), L"Mob");

			RazorBeakHP = new CGameObject;
			RazorBeakHP->SetName(L"NORTH_RazorBeak_Mini5_HP");
			RazorBeakHP->AddComponent(new CTransform);
			RazorBeakHP->AddComponent(new CMeshRender);
			RazorBeakHP->AddComponent(new CJungleMINIHPScript);
			RazorBeak->AddChild(RazorBeakHP);
		}
		//레드팀 블루
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_blue.fbx");
			CGameObject* Blue = nullptr;
			Blue = pMeshData->Instantiate();
			Blue->AddComponent(new CBlueScript);
			Blue->SetName(L"NORTH_Blue");
			Blue->Transform()->SetRelativeScale(0.22f, 0.22f, 0.22f);
			Blue->Transform()->SetRelativeRot(Vec3(0.f, XMConvertToRadians(-57.f), 0.f));
			CBlueScript* Script = Blue->GetScript<CBlueScript>();
			Script->SetAggroPos(Vec3(1653.f, 0.f, 1011.f));
			Script->SetUnitType(UnitType::NORTH_BLUE);
			SpawnGameObject(Blue, Vec3(1636, 10.f, 1009.f), L"Mob");

			CGameObject* BlueAggro = new CGameObject;
			BlueAggro->SetName(L"NORTH_BlueAggroRadius");
			BlueAggro->AddComponent(new CTransform);
			BlueAggro->AddComponent(new CCollider2D);
			BlueAggro->Transform()->SetRelativeScale(Vec3(170, 170.f, 170.f));
			BlueAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			BlueAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			BlueAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(BlueAggro, Vec3(1653.f, 0.f, 1011.f), 0);

			CGameObject* BlueHP = new CGameObject;
			BlueHP->SetName(L"NORTH_BlueHP");
			BlueHP->AddComponent(new CTransform);
			BlueHP->AddComponent(new CMeshRender);
			BlueHP->AddComponent(new CJungleMobHPScript);
			Blue->AddChild(BlueHP);
		}
		//레드팀 레드
		{
			Ptr<CMeshData> pMeshData = nullptr;
			pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\jungle_red.fbx");
			CGameObject* Red = nullptr;
			Red = pMeshData->Instantiate();
			Red->AddComponent(new CRedScript);
			Red->SetName(L"NORTH_Red");
			Red->Transform()->SetRelativeScale(0.35f, 0.35f, 0.35f);
			Red->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-54.f), XMConvertToRadians(-180.f)));
			CRedScript* Script = Red->GetScript<CRedScript>();
			Script->SetAggroPos(Vec3(1055, 15.f, 1637.f));
			Script->SetUnitType(UnitType::NORTH_RED);
			SpawnGameObject(Red, Vec3(1049, 15.f, 1607.f), L"Mob");

			CGameObject* RedAggro = new CGameObject;
			RedAggro->SetName(L"NORTH_RedAggroRadius");
			RedAggro->AddComponent(new CTransform);
			RedAggro->AddComponent(new CCollider2D);
			RedAggro->Transform()->SetRelativeScale(Vec3(190, 190.f, 190.f));
			RedAggro->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
			RedAggro->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			RedAggro->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
			SpawnGameObject(RedAggro, Vec3(1055, 15.f, 1637.f), 0);

			CGameObject* RedHP = new CGameObject;
			RedHP->SetName(L"NORTH_RedHP");
			RedHP->AddComponent(new CTransform);
			RedHP->AddComponent(new CMeshRender);
			RedHP->AddComponent(new CJungleMobHPScript);
			Red->AddChild(RedHP);
		}
	}
	//드래곤
	{
		Ptr<CMeshData> pMeshData = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Elder_Dragon.fbx");
		CGameObject* Dragon = nullptr;
		Dragon = pMeshData->Instantiate();
		Dragon->AddComponent(new CDragonScript);
		Dragon->SetName(L"Dragon");
		Dragon->Transform()->SetRelativeScale(0.33f, 0.33f, 0.33f);
		Dragon->Transform()->SetRelativeRot(Vec3(XMConvertToRadians(180.f), XMConvertToRadians(-44.f), XMConvertToRadians(-180.f)));
		CDragonScript* Script = Dragon->GetScript<CDragonScript>();
		Script->SetAggroPos(Vec3(1451, 15.f, 649.f));
		Script->SetAlertPos(Vec3(1483, 15.f, 689.f));
		Script->SetUnitType(UnitType::DRAGON);
		SpawnGameObject(Dragon, Vec3(1451, 0.f, 656.f), L"Mob");

		CGameObject* DragonAlertRadius = new CGameObject;
		DragonAlertRadius->SetName(L"DragonAlertRadius");
		DragonAlertRadius->AddComponent(new CTransform);
		DragonAlertRadius->AddComponent(new CCollider2D);
		DragonAlertRadius->Transform()->SetRelativeScale(Vec3(300, 300.f, 300.f));
		DragonAlertRadius->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		DragonAlertRadius->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
		DragonAlertRadius->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		SpawnGameObject(DragonAlertRadius, Vec3(1483, 15.f, 689.f), 0);

		CGameObject* DragonAggroRadius = new CGameObject;
		DragonAggroRadius->SetName(L"DragonAggroRadius");
		DragonAggroRadius->AddComponent(new CTransform);
		DragonAggroRadius->AddComponent(new CCollider2D);
		DragonAggroRadius->Transform()->SetRelativeScale(Vec3(200, 200.f, 200.f));
		DragonAggroRadius->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
		DragonAggroRadius->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
		DragonAggroRadius->Collider2D()->SetOffsetRot(Vec3(XMConvertToRadians(90.f), 0.f, 0.f));
		SpawnGameObject(DragonAggroRadius, Vec3(1451, 15.f, 649.f), 0);

	}

}
