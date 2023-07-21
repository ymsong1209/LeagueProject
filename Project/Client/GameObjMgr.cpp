#include "pch.h"
#include "GameObjMgr.h"

void GameObjMgr::AddPlayer(PlayerInfo _info, bool myPlayer)
{
	if (myPlayer)
	{
		// playerScript에 _info 변수들 추가하기.

		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = new CGameObject;

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"Jinx");

		pObj->AddComponent(new CPlayerScript);
		MyPlayerScript = pObj->GetScript<CPlayerScript>();
		pObj->AddComponent(new CPathFinder);

		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);

		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		SpawnGameObject(pObj, Vec3(0, 0, 0), 0);
	}
	else
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = new CGameObject;

		pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\Jinx.fbx");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"OtherJinx");

		pObj->AddComponent(new CPlayerScript);
		pObj->AddComponent(new CPathFinder);

		pObj->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Jinx");
		pObj->Animator3D()->SetRepeat(true);

		pObj->GetRenderComponent()->SetFrustumCheck(false);
		pObj->Transform()->SetRelativeScale(Vec3(0.3, 0.3, 0.3));

		SpawnGameObject(pObj, Vec3(100, 100, 0), 0);
	}
}

GameObjMgr::GameObjMgr()
{
}

GameObjMgr::~GameObjMgr()
{
}

